"""Gera o XML dos blocos (Tinkercad) de cada jogo. Uso: python3 blocos/gerar_xml.py"""
import pathlib

def b(tipo, campos='', valores='', corpo=''):
    return f'<block type="{tipo}">{campos}{valores}{corpo}</block>'
def campo(n, v): return f'<field name="{n}">{v}</field>'
def valor(n, x): return f'<value name="{n}">{x}</value>'
def stm(n, x): return f'<statement name="{n}">{x}</statement>' if x else ''
def num(n): return b('math_number', campo('NUM', n))
def txt(s): return b('text', campo('TEXT', s))
def var(v): return f'<block type="data_variable"><field name="VARIABLE" variabletype="">{v}</field></block>'
def cmp(op, a, c): return b('math_compare', campo('OP', op), valor('A', a) + valor('B', c))
def e(a, c): return b('logic_operation', campo('OP', 'AND'), valor('A', a) + valor('B', c))
def menos(a, c): return b('math_arithmetic', campo('OP', 'MINUS'), valor('A', a) + valor('B', c))
def ler(p): return b('input_read_digital', campo('PIN', p))
def ler_a(p): return b('input_read_analog', campo('PIN', p))
def dist(): return b('input_read_ultrasonic_distance', campo('PIN', 10) + campo('ECHO', 11) + campo('UNITS', 'CM'))
def mapear(x, lo, hi): return b('math_map', '', valor('IN', x) + valor('OUT_MIN', num(lo)) + valor('OUT_MAX', num(hi)))
def imprimir(x, linha=True): return b('output_print_serial', campo('NEWLINE', 'YES' if linha else 'NO'), valor('STRING', x))
def pino(p, alto): return b('output_set_digital_pin', campo('PIN', p) + campo('VALUE', 'HIGH' if alto else 'LOW'))
def som(nota, seg): return b('output_play_tone', campo('PIN', 8), valor('NOTE', num(nota)) + valor('DURATION', num(seg)))
def esperar(ms): return b('control_wait_seconds', campo('UNITS', 'MS'), valor('DURATION', num(ms)))
def definir(v, x): return f'<block type="data_setvariableto"><field name="VARIABLE" variabletype="">{v}</field>{valor("VALUE", x)}</block>'
def servo(p, x): return b('output_rotate_servo', campo('PIN', p), valor('ANGLE', x))
def se(cond, *corpo): return b('control_if', '', valor('CONDITION', cond), stm('DO', seq(*corpo)))
def enquanto(cond, *corpo): return b('control_while', campo('OP', 'WHILE'), valor('CONDITION', cond), stm('DO', seq(*corpo)))

def seq(*blocos):
    blocos = [x for x in blocos if x]
    if not blocos: return ''
    if len(blocos) == 1: return blocos[0]
    return blocos[0][:-len('</block>')] + '<next>' + seq(*blocos[1:]) + '</next></block>'

def cor(r, g, bl): return [pino(7, r), pino(6, g), pino(5, bl)]

def botao(p, texto, *extra):
    """Apertou: manda o texto para a tela e espera soltar (assim um aperto conta uma vez)."""
    return se(cmp('EQ', ler(p), num(0)), imprimir(txt(texto)), *extra, enquanto(cmp('EQ', ler(p), num(0)), esperar(5)))

def ouvir(*casos):
    """Ouvir a tela: lê o número que a tela mandou e faz o que ele pede."""
    return se(cmp('GT', b('input_serial_available'), num(0)),
              definir('cmd', b('input_serial_read')),
              *[se(cmp('EQ', var('cmd'), num(n)), *corpo) for n, corpo in casos])

def programa(variaveis, inicio, sempre):
    vs = ''.join(f'<variable type="">{v}</variable>' for v in variaveis)
    ini = f'<block type="control_on_start" x="20" y="20">{stm("DO", seq(*inicio))}</block>'
    sem = f'<block type="control_forever" x="20" y="140">{stm("DO", seq(*sempre))}</block>'
    return f'<xml xmlns="http://www.w3.org/1999/xhtml"><variables>{vs}</variables>{ini}{sem}</xml>'

FIM = [som(76, 0.15), esperar(170), som(83, 0.15), esperar(170), som(88, 0.3)]
JOGOS = {
    'nave': programa(['cmd'], [], [
        imprimir(txt('D'), False), imprimir(dist()),
        botao(2, 'B1'),
        ouvir((1, cor(1, 0, 0)), (2, cor(0, 1, 0)), (5, cor(0, 0, 0)),
              (6, [som(95, 0.02)]), (7, [som(84, 0.08)]), (8, [som(50, 0.4)])),
        esperar(30)]),
    'quiz': programa(['cmd'], [], [
        botao(2, 'B1'), botao(3, 'B2'),
        ouvir((1, [pino(7, 1), pino(6, 0), som(50, 0.4)]), (2, [pino(7, 0), pino(6, 1), som(84, 0.15)]),
              (5, [pino(7, 0), pino(6, 0)]), (6, [som(84, 0.02)])),
        esperar(10)]),
    'ritmo': programa(['cmd'], [], [
        botao(2, 'B1', som(60, 0.15)), botao(3, 'B2', som(62, 0.15)),
        botao(4, 'B3', som(64, 0.15)), botao(12, 'B4', som(67, 0.15)),
        ouvir((1, cor(1, 0, 0)), (2, cor(0, 1, 0)), (3, cor(0, 0, 1)), (5, cor(0, 0, 0))),
        esperar(5)]),
    'cabo': programa(['cmd'], [], [
        botao(2, 'B1'), botao(3, 'B2'),
        ouvir((1, cor(1, 0, 0)), (3, cor(0, 0, 1)), (4, cor(1, 1, 0)), (5, cor(0, 0, 0)),
              (6, [som(72, 0.06)]), (7, [som(88, 0.3)]), (9, FIM)),
        esperar(5)]),
    'bomba': programa(['cmd'], [], [
        imprimir(txt('P'), False), imprimir(ler_a('A0')),
        botao(2, 'B1'),
        ouvir((6, [*cor(1, 1, 0), som(95, 0.02), esperar(60), *cor(0, 0, 0)]),
              (7, [som(84, 0.12)]), (8, [som(52, 0.25)]),
              (9, [*cor(0, 1, 0), *FIM]), (10, [*cor(1, 0, 0), som(40, 1)])),
        esperar(40)]),
    'pescaria': programa(['cmd', 'distancia', 'fundo'], [definir('fundo', dist())], [
        servo(9, mapear(ler_a('A0'), 150, 60)),
        definir('distancia', dist()),
        se(e(cmp('GT', var('distancia'), num(0)), cmp('LT', var('distancia'), menos(var('fundo'), num(3)))),
           imprimir(txt('PONTO')), esperar(800)),
        ouvir((11, [definir('fundo', dist())]), (7, [som(90, 0.12)]), (6, [som(72, 0.06)]),
              (9, [som(76, 0.2), esperar(220), som(72, 0.2), esperar(220), som(69, 0.5)])),
        esperar(15)]),
}
if __name__ == '__main__':
    pasta = pathlib.Path(__file__).parent / 'xml'
    for nome, x in JOGOS.items():
        (pasta / f'{nome}.xml').write_text(x, encoding='utf-8')
        print(nome, len(x))
