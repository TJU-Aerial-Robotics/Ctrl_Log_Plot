1. depends:

    ubuntu 20.04 and qt5 (如果已安装ros, qt无需额外安装)

2. build:

```
pip install numpy scipy
```

```
cd build
qmake ../LogPlot.pro
make
./LogPlot
```

3. example:

4. TODO & bug：

the step response of position / velocity looks incorrect, but the programe works well for attitude control log...
