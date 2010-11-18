cd simpleplot
FOR /D %%p IN (simpleplot autoswitch freemesh enrichments graph axes plotlets mesh) DO (
  cd ../%%p
  qmake -project -o tmp.pri -after "TARGET=%%p"
  qmake %%p.pro -o %%p.vcproj
)
cd ../
