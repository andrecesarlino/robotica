// Peças comuns a todos os jogos: abas Montar/Jogar, manual de montagem e conexão com o Arduino.
// Cada jogo chama montarPagina({...}) e define Arduino.aoReceber = linha => {...}.

const Arduino = {
  aoReceber: () => {},
  porta: null,
  escritor: null,
  // Manda uma letra para o Arduino (acende LED, toca buzzer). Também acende o LED de mentira da tela.
  enviar(letra) {
    ledDaTela(letra);
    if (Arduino.escritor) Arduino.escritor.write(new TextEncoder().encode(letra)).catch(() => {});
  },
};

const CORES_LED = { r: '#ff3355', g: '#33ff77', b: '#3399ff', y: '#ffd23f' };
function ledDaTela(letra) {
  const led = document.getElementById('led');
  if (!led) return;
  if (letra === 'o') { led.style.background = '#333'; led.style.boxShadow = 'none'; }
  if (CORES_LED[letra]) { led.style.background = CORES_LED[letra]; led.style.boxShadow = `0 0 16px ${CORES_LED[letra]}`; }
}

async function conectar() {
  const status = document.getElementById('status');
  if (!('serial' in navigator)) {
    alert('Este navegador não conecta na USB. Abra no Google Chrome ou no Microsoft Edge.');
    return;
  }
  try {
    const porta = await navigator.serial.requestPort();
    await porta.open({ baudRate: 9600 });
    Arduino.porta = porta;
    Arduino.escritor = porta.writable.getWriter();
    status.textContent = 'Arduino conectado';
    const decodificador = new TextDecoder();
    let sobra = '';
    let fim = false;
    // erro de leitura (byte perdido, placa reiniciando) derruba só esse leitor; continua lendo
    while (porta.readable && !fim) {
      const leitor = porta.readable.getReader();
      try {
        while (true) {
          const { value, done } = await leitor.read();
          if (done) { fim = true; break; }
          const linhas = (sobra + decodificador.decode(value, { stream: true })).split('\n');
          sobra = linhas.pop();
          for (const linha of linhas) if (linha.trim()) Arduino.aoReceber(linha.trim());
        }
      } catch (e) {
        sobra = '';
      } finally {
        leitor.releaseLock();
      }
    }
    status.textContent = 'desconectado: o cabo saiu ou a placa reiniciou. Clique em Conectar de novo.';
  } catch (e) {
    status.textContent = e.name === 'NotFoundError' ? 'nenhuma porta escolhida'
      : 'não abriu a porta: feche o Monitor Serial da Arduino IDE e tente de novo';
  }
  Arduino.escritor = null;
}

function montarPagina(j) {
  document.title = j.titulo + ' · Robótica';
  const linhas = j.ligacoes.map(([peca, pino, obs]) =>
    `<tr><td>${peca}</td><td><span class="pino">${pino}</span></td><td>${obs || ''}</td></tr>`).join('');
  document.body.insertAdjacentHTML('afterbegin', `
  <header class="topo">
    <a class="voltar" href="./">← Jogos</a>
    <h1>${j.titulo}</h1>
    <nav class="abas"><button data-aba="montar">1. Montar</button><button data-aba="jogar">2. Jogar</button></nav>
  </header>
  <main class="montar" id="montar">
    <section class="caixa largo">
      <p class="desc">${j.descricao}</p>
      <ul class="pecas">${j.pecas.map(p => `<li>${p}</li>`).join('')}</ul>
    </section>
    <section class="caixa largo"><h2>Montagem</h2><img src="esquemas/${j.id}.svg" alt="Desenho da montagem"></section>
    <section class="caixa"><h2>Ligações</h2>
      <table><tr><th>Peça</th><th>Pino</th><th></th></tr>${linhas}</table>
    </section>
    <section class="caixa"><h2>Passo a passo</h2>
      <ol class="passos">
        <li>Prepare a protoboard como na etapa 0: 5V no + de baixo, GND no − de baixo e os trilhos de baixo ligados aos de cima.</li>
        <li>Monte as peças seguindo o desenho e a tabela.</li>
        <li>Copie o código abaixo, cole na Arduino IDE e envie para a placa.</li>
        <li><b>Feche o Monitor Serial</b> da IDE (senão a página não consegue usar a porta).</li>
        <li>Vá na aba <b>2. Jogar</b>, clique em <b>Conectar Arduino</b> e escolha a porta.</li>
        ${(j.passosExtras || []).map(p => `<li>${p}</li>`).join('')}
      </ol>
      ${j.tinkercad ? `<p>Circuito pronto no Tinkercad: <a href="${j.tinkercad}" target="_blank">abrir</a>. Lá dá para simular e testar pelo Monitor Serial; a tela do jogo só conecta no Arduino de verdade.</p>` : ''}
    </section>
    <section class="caixa largo codigo"><h2>Código do Arduino</h2>
      <button id="copiar">Copiar código</button>
      <pre id="codigo">carregando...</pre>
    </section>
  </main>
  <section class="jogar" id="jogar">
    <div class="barra">
      <button class="forte" id="conectar">Conectar Arduino</button>
      <span class="led" id="led" title="LED do Arduino"></span>
      <span class="status" id="status">sem Arduino: dá para testar pelo teclado</span>
      <span class="dica">${j.teclado}</span>
    </div>
    <canvas id="tela"></canvas>
  </section>`);

  const abas = document.querySelectorAll('.abas button');
  const mostrar = aba => {
    document.getElementById('montar').style.display = aba === 'montar' ? 'grid' : 'none';
    document.getElementById('jogar').style.display = aba === 'jogar' ? 'flex' : 'none';
    abas.forEach(b => b.classList.toggle('ativa', b.dataset.aba === aba));
    location.hash = aba;
  };
  abas.forEach(b => b.onclick = () => mostrar(b.dataset.aba));
  mostrar(location.hash === '#jogar' ? 'jogar' : 'montar');

  document.getElementById('conectar').onclick = conectar;
  fetch(`codigo/${j.id}/${j.id}.ino`).then(r => r.text()).then(t => document.getElementById('codigo').textContent = t);
  document.getElementById('copiar').onclick = async () => {
    await navigator.clipboard.writeText(document.getElementById('codigo').textContent);
    document.getElementById('copiar').textContent = 'Copiado!';
    setTimeout(() => document.getElementById('copiar').textContent = 'Copiar código', 1500);
  };

  // Teclado faz papel dos botões, para testar sem Arduino
  addEventListener('keydown', e => {
    if (e.repeat || !j.teclas) return;
    const linha = j.teclas[e.key.toLowerCase()];
    if (linha) { e.preventDefault(); Arduino.aoReceber(linha); }
  });
}

// Canvas do tamanho da tela, em pixels reais; devolve {ctx, W, H, s} a cada quadro
function prepararTela() {
  const tela = document.getElementById('tela');
  const ctx = tela.getContext('2d');
  const s = devicePixelRatio;
  const W = tela.width = tela.clientWidth * s;
  const H = tela.height = tela.clientHeight * s;
  return { tela, ctx, W, H, s };
}

function texto(ctx, t, x, y, tamanho, cor, alinhamento = 'center', peso = 700) {
  ctx.font = `${peso} ${tamanho}px 'Space Grotesk', Arial, sans-serif`;
  ctx.fillStyle = cor;
  ctx.textAlign = alinhamento;
  ctx.textBaseline = 'middle';
  ctx.fillText(t, x, y);
}

function recorde(id, pontos) {
  const chave = 'recorde-' + id;
  let r = 0;
  try { r = Number(localStorage.getItem(chave)) || 0; } catch (e) {}
  if (pontos > r) { r = pontos; try { localStorage.setItem(chave, r); } catch (e) {} }
  return r;
}

// Bip pelo computador, só quando testa pelo teclado (com Arduino quem toca é o buzzer)
let audio;
function bipTeste(freq, ms = 120) {
  if (Arduino.escritor) return;
  audio = audio || new AudioContext();
  const o = audio.createOscillator(), g = audio.createGain();
  o.frequency.value = freq;
  g.gain.value = 0.05;
  o.connect(g).connect(audio.destination);
  o.start();
  o.stop(audio.currentTime + ms / 1000);
}
