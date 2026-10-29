// Menu do topo, igual em todas as páginas
const MENU = [['./', 'Jogos'], ['aula.html#1', 'Aula 1'], ['aula.html#2', 'Aula 2'], ['aula.html#3', 'Aula 3'],
  ['aula.html#4', 'Aula 4'], ['aula.html#5', 'Aula 5']];
function menuHtml(ativo) {
  return `<nav class="menu"><a class="marca" href="./">Robótica</a>${MENU.map(([h, t]) =>
    `<a href="${h}"${t === ativo ? ' class="ativo"' : ''}>${t}</a>`).join('')}</nav>`;
}

// Peças comuns a todos os jogos: abas Montar/Jogar, manual de montagem e conexão com o Arduino.
// Cada jogo chama montarPagina({...}) e define Arduino.aoReceber = linha => {...}.

const Arduino = {
  aoReceber: () => {},
  porta: null,
  escritor: null,
  // Manda uma letra para o Arduino (acende LED, toca buzzer). Também acende o LED de mentira da tela.
  enviar(letra) {
    ledDaTela(letra);
    // os blocos do Tinkercad comparam números: a tela manda 1 para vermelho, 2 para verde...
    const n = NUMERO_DO_COMANDO[letra];
    if (Arduino.escritor && n) Arduino.escritor.write(new Uint8Array([n])).catch(() => {});
  },
};

const NUMERO_DO_COMANDO = { r: 1, g: 2, b: 3, y: 4, o: 5, t: 6, a: 7, e: 8, f: 9, x: 10, c: 11 };
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
    `<tr><td>${peca}${obs ? `<small>${obs}</small>` : ''}</td><td><span class="pino">${pino}</span></td></tr>`).join('');
  const passos = ['Montagem', 'Blocos', 'Jogar'];
  const nav = n => `<div class="navpasso">${n > 1 ? `<button data-ir="${n - 1}">← Voltar</button>` : '<span></span>'}${n < 3 ? `<button class="forte" data-ir="${n + 1}">Próximo: ${passos[n]} →</button>` : ''}</div>`;
  document.body.insertAdjacentHTML('afterbegin', `
  ${menuHtml('Jogos')}
  <div class="trilha"><a href="./">Jogos</a><span>›</span><b>${j.titulo}</b></div>
  <ol class="passos-nav">${passos.map((t, i) => `<li data-ir="${i + 1}"><span>${i + 1}</span>${t}</li>`).join('')}</ol>
  <main class="passo" data-passo="1">
    <div class="montagem">
      <figure><img src="${j.imagem || `montagem/${j.id}.png`}" alt="Montagem"><figcaption>
        ${j.tinkercad ? `<a href="${j.tinkercad}" target="_blank">Circuito no Tinkercad (para montar os blocos)</a> · ` : ''}<a href="esquemas/${j.id}.svg" target="_blank">Ver o desenho com as ligações</a></figcaption></figure>
      <div>
        <h2>Ligações</h2>
        <table>${linhas}</table>
        <ul class="avisos">
          <li>Antes de tudo: ligue o 5V e o GND do Arduino nas linhas + e − da protoboard.</li>
          ${(j.passosExtras || []).map(p => `<li>${p}</li>`).join('')}
        </ul>
      </div>
    </div>
    ${nav(1)}
  </main>
  <main class="passo" data-passo="2">
    ${j.semBlocos ? `<p class="dica-codigo">${j.semBlocos}</p>` : `
    <p class="dica-codigo">Monte estes blocos no Tinkercad${j.tinkercad ? ` (<a href="${j.tinkercad}" target="_blank">abrir o circuito</a>)` : ''}.</p>
    <img class="blocos-img" src="blocos/jogos/${j.id}.png" alt="Blocos do jogo">
    <div class="pullup">
      <b>Antes de enviar para a placa</b>
      <p>No Tinkercad, troque <b>Blocos</b> por <b>Blocos + Texto</b>, copie o texto e cole na Arduino IDE.${j.semBotao ? '' : ` Nos pinos dos botões, troque <code>INPUT</code> por <code>INPUT_PULLUP</code>:</p>
      <pre>pinMode(2, INPUT_PULLUP);</pre>
      <p>Assim o botão funciona sem resistor.`} Depois envie para a placa e feche o Monitor Serial.</p>
    </div>`}
    ${nav(2)}
  </main>
  <section class="jogar passo" data-passo="3" id="jogar">
    <div class="barra">
      <button class="forte" id="conectar">Conectar Arduino</button>
      <span class="led" id="led" title="LED do Arduino"></span>
      <span class="status" id="status"></span>
      <span class="dica">${j.teclado}</span>
    </div>
    <canvas id="tela"></canvas>
  </section>`);

  const ir = n => {
    n = Math.min(3, Math.max(1, Number(n) || 1));
    document.querySelectorAll('.passo').forEach(el => el.classList.toggle('ativo', el.dataset.passo == n));
    document.querySelectorAll('.passos-nav li').forEach(li => {
      li.classList.toggle('ativo', li.dataset.ir == n);
      li.classList.toggle('feito', li.dataset.ir < n);
    });
    history.replaceState(null, '', '#' + n);
    if (n === 3) ajustarTela();
    scrollTo(0, 0);
  };
  document.addEventListener('click', e => { const alvo = e.target.closest('[data-ir]'); if (alvo) ir(alvo.dataset.ir); });
  const pelaUrl = () => ir(location.hash === '#jogar' ? 3 : location.hash.slice(1));
  addEventListener('hashchange', pelaUrl);
  pelaUrl();

  // a tela do jogo ocupa o resto da janela
  function ajustarTela() {
    const el = document.getElementById('jogar');
    el.style.height = (innerHeight - el.getBoundingClientRect().top - scrollY) + 'px';
  }
  addEventListener('resize', ajustarTela);

  document.getElementById('conectar').onclick = conectar;
  // Teclado faz papel dos botões, só no passo Jogar
  addEventListener('keydown', e => {
    if (e.repeat || !j.teclas || !document.getElementById('jogar').classList.contains('ativo')) return;
    if (/^(INPUT|TEXTAREA|SELECT)$/.test(e.target.tagName)) return;  // digitando o nome da equipe, por exemplo
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
  if (!Arduino.escritor) som(freq, ms);
}
// Som pelo computador, toca mesmo com o Arduino conectado.
function som(freq, ms = 120, volume = 0.05, tipo = 'sine') {
  audio = audio || new AudioContext();
  const o = audio.createOscillator(), g = audio.createGain();
  o.type = tipo;
  o.frequency.value = freq;
  g.gain.value = volume;
  o.connect(g).connect(audio.destination);
  o.start();
  o.stop(audio.currentTime + ms / 1000);
}
