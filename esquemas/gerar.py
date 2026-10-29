"""Desenhos de montagem dos jogos, no padrão das aulas (usa o gerador do radar-arduino).

Uso: python3 esquemas/gerar.py  -> cria um .svg por jogo nesta pasta
"""
import pathlib
import sys

sys.path.insert(0, "/mnt/dados/projetos/arduino/radar-arduino/esquema")
from gerar_esquemas import (aula3, col, hole, pin, rail_n, rail_p, ROW, VERM, PRETO, LARANJA,
                            AMARELO, VERDE, ROXO, AZUL, MARROM, CINZA)

ROSA = "#e84393"


def base(titulo):
    e = aula3(0)  # etapa 0: 5V e GND nos trilhos de baixo, ligados aos de cima pela direita
    e.titulo = titulo
    return e


def botao(e, c, p, y, nome, cor=CINZA):
    """Botão no vão do meio: canto de cima no trilho - de cima, canto oposto no pino."""
    e.botao(c)
    e.fio(hole(c, "a"), (col(c), 153), PRETO)
    e.fio(hole(c + 2, "j"), pin(p), cor, via=[(col(c + 2), y), (pin(p)[0], y)])
    e.text(col(c + 1), ROW["h"] + 5, nome, 14, cor)


def buzzer(e, y=486):
    e.buzzer(20, "c")
    e.fio(hole(20, "e"), pin("8"), AMARELO, via=[(col(20), 290), (col(19), 290), (col(19), y), (pin("8")[0], y)])
    e.fio(hole(22, "e"), rail_n(22), PRETO)
    e.legenda.append((AMARELO, "Buzzer +", "pino 8 (buzzer - no trilho -)"))


def rgb(e):
    """LED RGB cátodo comum nas colunas 24-27, 300Ω em pé no vão; R, G, B nos pinos 7, 6, 5."""
    e.rgb(24)
    for c in (24, 26, 27):
        e.resistor_v(c, "300Ω")
    e.fio(hole(25, "d"), rail_n(25), PRETO, via=[(col(25), 286), (col(25) + 9, 300), (col(25) + 9, 395)])
    for k, (c, p, cor) in enumerate(((24, "7", VERM), (26, "~6", VERDE), (27, "~5", AZUL))):
        y = 444 + 6 * k
        e.fio(hole(c, "j"), pin(p), cor, via=[(col(c), y), (pin(p)[0], y)])
    e.legenda.append((AZUL, "LED RGB R, G, B (300Ω em pé)", "pinos 7, 6 e 5 (perna - no -)"))


def led(e, c, p, cor_led, cor_fio, y, nome):
    """LED com 300Ω deitado, igual ao sensor de ré."""
    e.led(c, "d", cor_led)
    e.resistor(c + 1, c + 4, "a", "300Ω")
    e.fio(hole(c + 4, "e"), rail_n(c + 4), PRETO, via=[(col(c + 4) + 9, 286), (col(c + 4) + 9, 395)])
    e.fio(hole(c, "e"), pin(p), cor_fio, via=[(col(c), y), (pin(p)[0], y)])
    e.text(col(c) + 9, ROW["h"] + 5, nome, 14, cor_fio)


def ultrassonico(e, sx=120, sy=250, nota=None):
    e.add(f'<rect x="{sx}" y="{sy}" width="164" height="72" rx="4" fill="#1f5fa8" stroke="#123" stroke-width="2"/>')
    for cx in (sx + 30, sx + 134):
        e.add(f'<circle cx="{cx}" cy="{sy + 30}" r="22" fill="#ccc" stroke="#888" stroke-width="3"/>')
        e.add(f'<circle cx="{cx}" cy="{sy + 30}" r="14" fill="#555"/>')
    e.text(sx + 82, sy - (30 if nota else 12), "Sensor ultrassônico", 16)
    if nota:
        e.text(sx + 82, sy - 12, nota, 13, "#555", weight="normal")
    xs = [sx + 55 + 18 * i for i in range(4)]
    for x, nome in zip(xs, ["VCC", "Trig", "Echo", "GND"]):
        e.add(f'<rect x="{x - 2}" y="{sy + 72}" width="4" height="18" fill="#999"/>')
        e.text(x + 4, sy + 68, nome, 9, "#fff", anchor="start", rot=-90)
    yb = sy + 90
    e.fio((xs[0], yb), rail_p(3), VERM, via=[(xs[0], 445), (col(3), 445)])
    e.fio((xs[1], yb), pin("~10"), VERDE)
    e.fio((xs[2], yb), pin("~11"), ROXO)
    e.fio((xs[3], yb), rail_n(2), PRETO, via=[(xs[3], 438), (col(2) - 4, 438)])
    e.legenda.append((VERDE, "Ultrassônico Trig / Echo (roxo)", "pinos 10 / 11 (VCC no +, GND no -)"))


def nave():
    e = base("Jogo da nave: ultrassônico, botão, buzzer e LED RGB")
    ultrassonico(e, nota="(fica deitado, olhando para cima)")
    botao(e, 13, "2", 470, "Atirar")
    e.legenda.append((CINZA, "Botão (atirar)", "pino 2 (outro canto no - de cima)"))
    buzzer(e)
    rgb(e)
    return e


def quiz():
    e = base("Quiz de botões: 2 botões, 2 LEDs e buzzer")
    botao(e, 0, "2", 470, "A", AZUL)
    botao(e, 4, "~3", 476, "B", ROSA)
    e.legenda.append((AZUL, "Botões A / B (rosa)", "pinos 2 / 3 (outro canto no - de cima)"))
    led(e, 8, "~6", "#2ecc40", VERDE, 450, "Verde")
    led(e, 14, "7", "#e33", VERM, 458, "Vermelho")
    e.legenda.append((VERDE, "LED verde / vermelho (+)", "pinos 6 / 7 (300Ω ao trilho -)"))
    buzzer(e)
    return e


def ritmo():
    e = base("Jogo de ritmo: 4 botões, buzzer e LED RGB")
    for k, (c, p, cor) in enumerate(((0, "2", AZUL), (4, "~3", ROSA), (8, "4", LARANJA), (13, "12", ROXO))):
        botao(e, c, p, 466 + 6 * k, str(k + 1), cor)
    e.legenda.append((AZUL, "Botões 1, 2, 3 e 4", "pinos 2, 3, 4 e 12 (outro canto no -)"))
    buzzer(e, 492 - 4)
    rgb(e)
    return e


def cabo():
    e = base("Cabo de guerra: 2 botões, buzzer e LED RGB")
    botao(e, 0, "2", 470, "Jogador 1", VERM)
    botao(e, 13, "~3", 476, "Jogador 2", AZUL)
    e.legenda.append((VERM, "Botão jogador 1 / 2 (azul)", "pinos 2 / 3 (outro canto no -)"))
    buzzer(e)
    rgb(e)
    return e


def bomba():
    e = base("Desarme a bomba: potenciômetro, botão, buzzer e LED RGB")
    e.potenciometro(2, "c")
    e.fio(hole(2, "e"), rail_p(2), VERM)
    e.fio(hole(4, "e"), rail_n(4), PRETO)
    e.fio_analogico("A0", hole(3, "e"), VERDE)
    e.legenda.append((VERDE, "Potenciômetro (meio)", "A0 (lados nos trilhos + e -)"))
    botao(e, 8, "2", 470, "Travar")
    e.legenda.append((CINZA, "Botão (travar)", "pino 2 (outro canto no - de cima)"))
    buzzer(e)
    rgb(e)
    return e


def pescaria():
    e = base("Pescaria: garra, joystick, ultrassônico e buzzer")
    # servo com a garra em cima à esquerda, como na aula 4
    vx, vy = 360, 110
    e.add(f'<rect x="{vx}" y="{vy}" width="120" height="58" rx="5" fill="#2a6fd6" stroke="#123" stroke-width="2"/>')
    e.add(f'<circle cx="{vx + 30}" cy="{vy + 29}" r="16" fill="#eee" stroke="#999" stroke-width="2"/>')
    e.add(f'<rect x="{vx + 26}" y="{vy - 4}" width="8" height="66" rx="4" fill="#fff" stroke="#999" stroke-width="1.5"/>')
    e.text(vx + 60, vy - 12, "Servo com a garra", 16)
    xd = vx + 120
    e.fio((xd, vy + 15), (col(2), 135), VERM)
    e.fio((xd, vy + 29), (col(1), 153), MARROM)
    e.fio((xd, vy + 43), pin("~9"), LARANJA, via=[(xd + 10, vy + 43), (xd + 10, 458), (pin("~9")[0], 458)])
    e.legenda += [(LARANJA, "Servo da garra, laranja", "pino 9 (os outros nos trilhos de cima)")]
    # joystick: só GND, +5V e VRx
    jx, jy = 110, 84
    e.add(f'<rect x="{jx}" y="{jy}" width="140" height="104" rx="8" fill="#222" stroke="#000" stroke-width="2"/>')
    e.add(f'<circle cx="{jx + 52}" cy="{jy + 52}" r="34" fill="#444" stroke="#666" stroke-width="3"/>')
    e.add(f'<circle cx="{jx + 52}" cy="{jy + 52}" r="20" fill="#111" stroke="#555" stroke-width="2"/>')
    e.text(jx + 52, jy - 10, "Joystick", 16)
    ys = [jy + 16 + 18 * i for i in range(5)]
    for y, nome in zip(ys, ["GND", "+5V", "VRx", "VRy", "SW"]):
        e.add(f'<rect x="{jx + 140}" y="{y - 2}" width="16" height="4" fill="#999"/>')
        e.text(jx + 134, y + 4, nome, 10, "#fff", anchor="end")
    xp = jx + 156
    e.fio((xp, ys[0]), (col(4), 153), PRETO, via=[(xp + 56, ys[0]), (xp + 56, 70), (col(4), 70)])
    e.fio((xp, ys[1]), (col(5), 135), VERM, via=[(xp + 40, ys[1]), (xp + 40, 60), (col(5), 60)])
    e.fio((xp, ys[2]), pin("A0"), VERDE, via=[(xp + 24, ys[2]), (xp + 24, 205), (40, 205), (40, 784), (pin("A0")[0], 784)])
    e.legenda.append((VERDE, "Joystick VRx", "A0 (+5V e GND nos trilhos de cima)"))
    ultrassonico(e, sx=120, sy=290, nota="(em cima do pote, olhando para o fundo)")
    buzzer(e)
    return e


def display():
    """Display de 1 dígito com o CI 4511. O Arduino manda o número em binário (A, B, C, D nos pinos 2 a 5)
    e o 4511 acende os segmentos. Chip nas colunas 2-9 e display nas 18-22, os dois em cima do vão.
    Um resistor de 300Ω só, no comum do display."""
    e = base("Display 0 a 9 com o CI 4511")
    def dip(c0, n, rotulo, cima, baixo, cor):
        x0, x1 = col(c0) - 9, col(c0 + n - 1) + 9
        e.add(f'<rect x="{x0}" y="{ROW["e"] - 4}" width="{x1 - x0}" height="{ROW["f"] - ROW["e"] + 8}" rx="4" fill="{cor}" stroke="#000" stroke-width="2"/>')
        e.text((x0 + x1) / 2, (ROW["e"] + ROW["f"]) / 2 + 5, rotulo, 13, "#fff")
        for i, t in enumerate(cima):
            e.text(col(c0 + i), ROW["e"] - 9, t, 10, "#14262a")
        for i, t in enumerate(baixo):
            e.text(col(c0 + i), ROW["f"] + 17, t, 10, "#14262a")
    # 4511: em cima (linha e) pinos 16 a 9, embaixo (linha f) pinos 1 a 8; chanfro à esquerda
    dip(2, 8, "CI 4511", ["5V", "f", "g", "a", "b", "c", "d", "e"], ["B", "C", "LT", "BL", "LE", "D", "A", "GND"], "#222")
    # display: em cima g f C a b, embaixo e d C c ponto
    dip(18, 5, "8", ["g", "f", "C", "a", "b"], ["e", "d", "C", "c", "."], "#333")
    # alimentação e pinos de controle do 4511
    e.fio(hole(2, "a"), (col(2), 135), VERM)                      # 5V (pino 16) no + de cima
    e.fio(hole(9, "j"), rail_n(9), PRETO)                          # GND (pino 8)
    e.fio(hole(4, "j"), rail_p(4), VERM)                           # LT no 5V
    e.fio(hole(5, "j"), rail_p(5), VERM)                           # BL no 5V
    e.fio(hole(6, "j"), rail_n(6), PRETO)                          # LE no GND
    # resistor do comum do display (coluna 20) até o trilho - de cima
    x, top_n = col(20), 153
    e.perna(x, ROW["a"], x, top_n)
    e.add(f'<rect x="{x - 7}" y="{top_n + 6}" width="14" height="26" rx="6" fill="#e3c99a" stroke="#8a6d3b" stroke-width="1.5"/>')
    for i, cr in enumerate(["#f28c28", "#111", "#7a4a1e"]):
        e.add(f'<rect x="{x - 7}" y="{top_n + 10 + i * 6}" width="14" height="3" fill="{cr}"/>')
    # saídas do 4511 (metade de cima) até os segmentos do display
    cores = {"a": VERM, "b": LARANJA, "c": AMARELO, "d": VERDE, "e": AZUL, "f": ROXO, "g": ROSA}
    saida = {"f": 3, "g": 4, "a": 5, "b": 6, "c": 7, "d": 8, "e": 9}
    disp_cima = {"g": 18, "f": 19, "a": 21, "b": 22}
    disp_baixo = {"e": 18, "d": 19, "c": 21}
    for k, (seg, cc) in enumerate(disp_cima.items()):
        y = ROW["a"] - 70 + 9 * k
        e.fio(hole(saida[seg], "b"), hole(cc, "b"), cores[seg], via=[(col(saida[seg]), y), (col(cc), y)])
    for k, (seg, cc) in enumerate(disp_baixo.items()):
        y = ROW["a"] - 100 + 9 * k
        xr = col(24) + 8 * k
        e.fio(hole(saida[seg], "c"), hole(cc, "i"), cores[seg],
              via=[(col(saida[seg]) + 4, y), (xr, y), (xr, ROW["j"] + 30), (col(cc), ROW["j"] + 30)])
    # Arduino -> entradas A, B, C, D do 4511 (metade de baixo)
    for k, (c, p, cor) in enumerate(((8, "2", VERM), (2, "~3", LARANJA), (3, "4", AMARELO), (7, "~5", VERDE))):
        y = 452 + 7 * k
        e.fio(hole(c, "j"), pin(p), cor, via=[(col(c), y), (pin(p)[0], y)])
    e.legenda = e.legenda[:2] + [(VERM, "4511 entradas A, B, C, D", "pinos 2, 3, 4, 5 do Arduino"),
                                 (ROXO, "4511 saídas a até g", "segmentos a até g do display"),
                                 (VERM, "4511 5V, LT e BL", "5V (trilho +)"),
                                 (PRETO, "4511 GND e LE", "GND (trilho -)"),
                                 (PRETO, "Display comum (C)", "resistor 300Ω ao trilho - de cima")]
    return e


JOGOS = {"display": display, "nave": nave, "quiz": quiz, "ritmo": ritmo, "cabo": cabo, "bomba": bomba, "pescaria": pescaria}

if __name__ == "__main__":
    pasta = pathlib.Path(__file__).parent
    for nome, f in JOGOS.items():
        (pasta / f"{nome}.svg").write_text(f().svg(), encoding="utf-8")
        print(nome)
