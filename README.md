# Plate_Labeler
Muy **basica** &amp; **Simple** app para anotar las patentes de las imagenes y guardarlas en archivo. Hecha con C++ y QT.

## GUI

![alt text](https://i.imgur.com/5xbOOM3.png)

Ejemplo de uso: si sequiere construir un **CNN+RNN** o algo de ese estilo para reconocer patentes y se requiere anotar patentes locales (porque difieren mucho de datasets de otros paises) entonces esta simple herramienta puede ser útil para hacer las anotaciones de las patentes.

## Uso

Copiar todas las imagenes en la carpeta `Plate_Labeler/imgs/` y luego correr la aplicación.
Las anotaciones se guarda en el archivo results.txt (separado por tab)

## Shortcuts

`ESC` se borra/descarta la imagen actual

`ctrl + enter` se guarda la anotacion y se procede a la siguiente imagen
