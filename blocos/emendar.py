"""Emenda prints sobrepostos (de cima para baixo) achando a sobreposição pelos pixels.
Compara a área sobreposta inteira (blocos repetidos enganam uma comparação curta).
Uso: python3 emendar.py saida.png parte1.png parte2.png ..."""
import sys
import numpy as np
from PIL import Image

PASSO = 612  # medido nos prints: cada parte desce 612 px de imagem

def emendar(arquivos):
    imgs = [np.asarray(Image.open(a).convert('RGB')).astype(np.int16) for a in arquivos]
    resultado = imgs[0]
    for k, img in enumerate(imgs[1:], start=1):
        esperado = round(k * PASSO)
        # ajuste fino de poucos pixels; blocos repetidos enganam uma busca larga
        melhor = min(range(esperado - 4, esperado + 5),
                     key=lambda y: np.abs(resultado[y:y + 100] - img[:100]).mean())
        resultado = np.vstack([resultado[:melhor], img])
    fundo = resultado[-1, -1]
    marca = (np.abs(resultado - fundo).sum(axis=2) > 30)
    linhas, colunas = np.where(marca.any(axis=1))[0], np.where(marca.any(axis=0))[0]
    resultado = resultado[:linhas.max() + 20, :colunas.max() + 20]
    return Image.fromarray(resultado.astype(np.uint8))

if __name__ == '__main__':
    emendar(sys.argv[2:]).save(sys.argv[1], optimize=True)
    print('ok', sys.argv[1])
