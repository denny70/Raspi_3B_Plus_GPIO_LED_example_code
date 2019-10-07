# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'widget.ui'
#
# Created by: PyQt5 UI code generator 5.12.3
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Widget(object):
    def setupUi(self, Widget):
        Widget.setObjectName("Widget")
        Widget.resize(400, 300)
        self.ok_btn = QtWidgets.QPushButton(Widget)
        self.ok_btn.setGeometry(QtCore.QRect(200, 190, 113, 32))
        self.ok_btn.setObjectName("ok_btn")
        self.label = QtWidgets.QLabel(Widget)
        self.label.setGeometry(QtCore.QRect(30, 100, 101, 21))
        self.label.setObjectName("label")
        self.GPIO_port = QtWidgets.QLineEdit(Widget)
        self.GPIO_port.setGeometry(QtCore.QRect(140, 100, 61, 25))
        self.GPIO_port.setObjectName("GPIO_port")
        self.on_off = QtWidgets.QLineEdit(Widget)
        self.on_off.setGeometry(QtCore.QRect(310, 100, 71, 25))
        self.on_off.setObjectName("on_off")
        self.label_2 = QtWidgets.QLabel(Widget)
        self.label_2.setGeometry(QtCore.QRect(230, 100, 67, 17))
        self.label_2.setObjectName("label_2")

        self.retranslateUi(Widget)
        QtCore.QMetaObject.connectSlotsByName(Widget)

    def retranslateUi(self, Widget):
        _translate = QtCore.QCoreApplication.translate
        Widget.setWindowTitle(_translate("Widget", "Widget"))
        self.ok_btn.setText(_translate("Widget", "Ok"))
        self.label.setText(_translate("Widget", "GPIO: number:"))
        self.GPIO_port.setText(_translate("Widget", "2"))
        self.on_off.setText(_translate("Widget", "1"))
        self.label_2.setText(_translate("Widget", "on:1 off:0"))
