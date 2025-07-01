import serial, numpy as np, pyqtgraph as pg
from pyqtgraph.Qt import QtWidgets, QtCore

PORT = '/dev/cu.usbserial-110'  # 明示的にArduino Unoのポートを指定
BAUD = 115200
N_BINS = 63                    # Arduino側で送ったビン数

ser = serial.Serial(PORT, BAUD, timeout=1)

app = QtWidgets.QApplication([])
win   = pg.GraphicsLayoutWidget(title='Live FFT')
plot  = win.addPlot()
plot.setYRange(0, 200)          # 振幅スケールを適宜調整
curve = plot.plot(pen='y')
win.show()

def update():
    line = ser.readline().decode().strip()
    if not line:          # 空行回避
        return
    data = np.fromstring(line, dtype=float, sep=',')
    if data.size != N_BINS:
        return            # 欠落行は破棄
    curve.setData(data)

timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(20)           # ≈50 fps
if __name__ == '__main__':
    QtWidgets.QApplication.instance().exec()