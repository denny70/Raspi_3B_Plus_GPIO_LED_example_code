
import sys
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot, QProcess

import widget

class Main(QWidget, widget.Ui_Widget):
    def __init__(self):
        super(QWidget, self).__init__()
        self.setupUi(self)

        
        self.process = QProcess()

        self.ok_btn.clicked.connect(self.sendButtonClick)
    
    @pyqtSlot()
    def sendButtonClick(self):

        self.gpio_port = self.GPIO_port.text()
        self.gpio_onOff = self.on_off.text()
        
        self.process = QProcess()
        cmd = str("../a.out -w " + self.gpio_port + " " +self.gpio_onOff)
        self.process.start(cmd)
        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = Main()
    window.show()
    app.exec_()
    #sys.exit(app.exec_())
        

