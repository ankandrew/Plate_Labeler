# Plate_Labeler
Muy **basica** &amp; **Simple** app para anotar las patentes de las imagenes y guardarlas en archivo. Hecha con C++ y QT.

## GUI

![alt text](https://i.imgur.com/5xbOOM3.png)

Ejemplo de uso: si sequiere construir un **CNN+RNN** o algo de ese estilo para reconocer patentes y se requiere anotar patentes locales (porque difieren mucho de datasets de otros paises) entonces esta simple herramienta puede ser útil para hacer las anotaciones de las patentes.

## Uso

Copiar todas las imagenes en la carpeta `Plate_Labeler/imgs/` y luego correr la aplicación.
Las anotaciones se guarda en el archivo results.txt (separado por tab)

Las imagenes anotadas se muevan a `done_imgs/`

## Shortcuts

`ESC` se borra/descarta la imagen actual

`ctrl + enter` / `enter` se guarda la anotacion y se procede a la siguiente imagen

## Pseudo-Labeling

Si existe un archivo `img_pred.txt` se utilizara la informacion de este para sugerir patentes. El formato del archivo separado por `\t` es:

`nombre_de_la_imagen.png  PATENTE  CONFIANZA`

ejemplo:

`patente2.png  ABC123DE  67.32`

De esta forma se pueden guardar en `img_pred.txt` las predicciones hechas por un modelo OCR inicial. Aun cuando no tenga buena precision puede completar algunos caracteres y agilizar tiempo.

## Prevenir duplicados

Si la patente ya esta registrada mas de 2 veces, se notifica con un MsgBox de confirmacion

## Usos Practicos

*Esta aplicacion es la que se utiliza para labelear las patentes para este [proyecto](https://github.com/ankandrew/cnn-ocr-lp)*
