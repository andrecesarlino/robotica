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
    `<tr><td>${peca}${obs ? `<small>${obs}</small>` : ''}</td><td><span class="pino">${pino}</span></td></tr>`).join('');
  const passos = ['Peças', 'Montagem', 'Código', 'Jogar'];
  const nav = n => `<div class="navpasso">${n > 1 ? `<button data-ir="${n - 1}">← Voltar</button>` : '<span></span>'}${n < 4 ? `<button class="forte" data-ir="${n + 1}">Próximo: ${passos[n]} →</button>` : ''}</div>`;
  document.body.insertAdjacentHTML('afterbegin', `
  ${menuHtml('Jogos')}
  <div class="trilha"><a href="./">Jogos</a><span>›</span><b>${j.titulo}</b></div>
  <ol class="passos-nav">${passos.map((t, i) => `<li data-ir="${i + 1}"><span>${i + 1}</span>${t}</li>`).join('')}</ol>
  <main class="passo" data-passo="1">
    <p class="desc">${j.descricao}</p>
    <h2>Separe as peças</h2>
    <ul class="pecas">${j.pecas.map(p => `<li>${p}</li>`).join('')}</ul>
    <p class="nota">Além do Arduino, da protoboard e dos fios.</p>
    ${nav(1)}
  </main>
  <main class="passo" data-passo="2">
    <div class="montagem">
      <figure><img src="${j.imagem || `montagem/${j.id}.png`}" alt="Montagem"><figcaption>
        ${j.tinkercad ? `<a href="${j.tinkercad}" target="_blank">Abrir no Tinkercad</a> · ` : ''}<a href="esquemas/${j.id}.svg" target="_blank">Ver o desenho com as ligações</a></figcaption></figure>
      <div>
        <h2>Ligações</h2>
        <table>${linhas}</table>
        <ul class="avisos">
          <li>Antes das peças: 5V no + e GND no − da protoboard.</li>
          ${(j.passosExtras || []).map(p => `<li>${p}</li>`).join('')}
        </ul>
      </div>
    </div>
    ${nav(2)}
  </main>
  <main class="passo" data-passo="3">
    <p class="dica-codigo">Digite as partes na ordem, uma embaixo da outra, na Arduino IDE. Depois envie para a placa e <b>feche o Monitor Serial</b>, senão a página não conecta.</p>
    <div id="partes"></div>
    <details class="inteiro"><summary>Ver o código inteiro</summary><pre id="codigo">carregando...</pre></details>
    ${nav(3)}
  </main>
  <section class="jogar passo" data-passo="4" id="jogar">
    <div class="barra">
      <button class="forte" id="conectar">Conectar Arduino</button>
      <span class="led" id="led" title="LED do Arduino"></span>
      <span class="status" id="status">sem Arduino: dá para testar pelo teclado</span>
      <span class="dica">${j.teclado}</span>
    </div>
    <canvas id="tela"></canvas>
  </section>`);

  const ir = n => {
    n = Math.min(4, Math.max(1, Number(n) || 1));
    document.querySelectorAll('.passo').forEach(el => el.classList.toggle('ativo', el.dataset.passo == n));
    document.querySelectorAll('.passos-nav li').forEach(li => {
      li.classList.toggle('ativo', li.dataset.ir == n);
      li.classList.toggle('feito', li.dataset.ir < n);
    });
    history.replaceState(null, '', '#' + n);
    if (n === 4) ajustarTela();
    scrollTo(0, 0);
  };
  document.addEventListener('click', e => { const alvo = e.target.closest('[data-ir]'); if (alvo) ir(alvo.dataset.ir); });
  const pelaUrl = () => ir(location.hash === '#jogar' ? 4 : location.hash.slice(1));
  addEventListener('hashchange', pelaUrl);
  pelaUrl();

  // a tela do jogo ocupa o resto da janela
  function ajustarTela() {
    const el = document.getElementById('jogar');
    el.style.height = (innerHeight - el.getBoundingClientRect().top - scrollY) + 'px';
  }
  addEventListener('resize', ajustarTela);

  document.getElementById('conectar').onclick = conectar;
  // O .ino tem linhas "//## Título | explicação" marcando o começo de cada parte
  fetch(`codigo/${j.id}/${j.id}.ino`, { cache: 'no-cache' }).then(r => r.text()).then(t => {
    const partes = [];
    for (const linha of t.split('\n')) {
      const m = linha.match(/^\/\/## (.*?) \| (.*)$/);
      if (m) partes.push({ titulo: m[1], explica: m[2], linhas: [] });
      else if (partes.length) partes[partes.length - 1].linhas.push(linha);
    }
    const limpo = l => l.join('\n').replace(/^\n+|\n+$/g, '');
    const esc = x => x.replace(/&/g, '&amp;').replace(/</g, '&lt;');
    document.getElementById('partes').innerHTML = partes.map((p, i) => `
      <section class="parte"><div class="parte-topo"><span>${i + 1}</span><b>${p.titulo}</b><em>${p.explica}</em></div>
      <pre>${esc(limpo(p.linhas))}</pre></section>`).join('');
    document.getElementById('codigo').textContent = t.split('\n').filter(l => !l.startsWith('//##')).join('\n');
  });

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
  if (Arduino.escritor) return;
  audio = audio || new AudioContext();
  const o = audio.createOscillator(), g = audio.createGain();
  o.frequency.value = freq;
  g.gain.value = 0.05;
  o.connect(g).connect(audio.destination);
  o.start();
  o.stop(audio.currentTime + ms / 1000);
}
