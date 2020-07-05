import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
  id: window
  visible: true
  width: 768
  height: 768
  title: qsTr("Sequencer")

  menuBar: ToolBar {
    contentHeight: toolButton.implicitHeight

    ToolButton {
      id: toolButton
      text: "\u2630"
      font.pixelSize: Qt.application.font.pixelSize * 1.6
      onClicked: {
        drawer.open()
      }
    }

    ToolButton {
      id: backButton
      text: "\u25C0"
      font.pixelSize: Qt.application.font.pixelSize * 1.6
      anchors.right: parent.right
      onClicked: {
        stackView.pop()
      }
    }

    Label {
      text: stackView.currentItem.title
      anchors.centerIn: parent
    }
  }

  Drawer {
    id: drawer
    width: window.width * 0.33
    height: window.height

    Column {
      anchors.fill: parent

      ItemDelegate {
        text: qsTr("Home")
        width: parent.width
        onClicked: {
          stackView.push("home.ui.qml")
          drawer.close()
        }
      }

      ItemDelegate {
        text: qsTr("List")
        width: parent.width
        onClicked: {
          stackView.push("list.ui.qml")
          drawer.close()
        }
      }

      ItemDelegate {
        text: qsTr("Quit")
        width: parent.width
        onClicked: {
          Qt.quit()
        }
      }
    }
  }

  StackView {
    id: stackView
    initialItem: "home.ui.qml"
    anchors.fill: parent
  }
}
