# Para trabajar:
## Instalación
Van a necesitar git , texlive (o algun compilador de latex) y algunos paquetes adicionales.
**en distribuciones debian**:
```
sudo apt install git
sudo apt install texlive
sudo apt install texlive-formats-extra
sudo apt install texlive-extra-utils
```
## Empezar :D
En la carpeta que quieran tener el repositorio.
```
git clone https://github.com/Ignagx/Ingenieria-Electronica.git
cd Ingenieria-Electronica/
git switch TP2-ASyS
cd ASyS/TP2/
```
***Primero deben cambiarse a la rama en la que les corresponda trabajar segun el ejercicio***
>Los nombres de las ramas van a segir el formato: TP2-ASyS-ej*

si la rama existe:
```
git switch <rama>
```

Si tienen que crear la rama:
```
git switch -C <rama a crear>
```
Para subir la nueva rama al repositorio:
```
git push -u origin <rama>
```
***configurar git***

Para poder commitear necesitan especificar quien carajo son, para esto:
```
git config --global user.name <nombre>
git config --global user.email <su mail (?>
```
Para que git les mergee automaticamente los commit que realicen otros compañeros sobre la rama:
```
git config pull.rebase true
```
No olviden hacer pull **siempre** antes de ponerse a modificar algo.

Si tienen problemas con el auto-merge y tienene que mergear manualmente me avisan y les explico, **no entrar en panico**.
De todas formas no deberia pasar si trabajan ordenadamente y no modifican distintas personas la misma linea al mismo 
tiempo.

Para el resto de cosas de git busquense un tutorial pq bueno una paja, pero con eso ya pueden empezar a trabajar sobre
el .tex.
***Para compilar el archivo .tex***
```
pdflatex tp2.tex
```
# Normas de convivencia !!
+ Por favor no hacer lineas infinitas, maximo 120 caracteres. en vim pueden marcar el caracter con :
```
set colorcolumn=120
```
+ Las imagenes, por cuestiones de formateo y tiempo de compilacion, se van a agregar como ultimo paso. De momento dejen
la imagen en /TP2/Imagenes y coloquen un comentario en el archivo .tex donde deba ser colocada la imagen.
+ No pushear codigo con warnings/errores de compilacion. (gaston)
+ git tiene problemas para mergear los archivos .pdf, asi que les va a pedir que hagan el merge manualmente si tienen 
cambios sobre el pdf y alguien mas lo pusheo, para evitar esto recomiendo (amenaza de muerte) que 
***nadie pushee el PDF***, y cuando este terminado se sube.
+ Tabulaciones de ***2(DOS) espacios***. en vim, colocan en el .vimrc:
```
" Tabs
set smarttab
" 1 tab == 2 spaces, auto indent
set tabstop=2 shiftwidth=2 expandtab noai
```
+ No dejar espacios tirados por ahi, para que el vim se los marque:
```
set list listchars=tab:»·,trail:·    
```
+ Poner buenos titulos de commit, se permite boludear si son graciosos (mati por favor NO).
+ Sub indices siempre en **minusculas**
+ Para las ecuaciones usar los entornos:
```
$$ <ecuacion> $$
```
Y si es necesario alinear ecuaciones:
```
$$
\begin{aligned}
 f(x) &= x^2 + 2x + 1 \\
 g(x) &= x^3 - 3x + 2
\end{aligned}
$$
```
(El caracter & es el caracrer de alineacion, en este caso las funciones sea alinearian por el '=')
