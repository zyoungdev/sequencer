import QtQuick 2.15
import QtQuick.Controls 2.2
// import com.app 1.0 // Wav
import QtGraphicalEffects 1.12 // RadialGradient
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12
import Qt.labs.platform 1.1

Page {
  function resetRandomOptionsBoxPosition() {
    randomOptionsBox.x = page.width / 2 - randomOptionsBox.width / 2
    randomOptionsBox.y = page.height / 2 - randomOptionsBox.height / 2
  }

  id: page
  onWidthChanged: resetRandomOptionsBoxPosition()
  onHeightChanged: onWidthChanged

  /* top controls */
  Row {
    anchors.bottom: padGrid.top;
    anchors.horizontalCenter: padGrid.horizontalCenter;

    /* play/stop */
    Button {
      id: playStop
      property bool isEngaged

      text: isEngaged ? "Stop" : "Play";
      highlighted: isEngaged
      onPressed: isEngaged = isEngaged ? engineModel.stop() : engineModel.play();
    }

    /* tempo */
    SpinBox {
      value: engineModel.bpm
      editable: true
      stepSize: 1
      from: 30
      to: 300


      onValueModified: engineModel.bpm = value;
    }

    /* grid size */
    SpinBox {
      value: padModel.padSize
      editable: true
      stepSize: 1
      from: 1
      to: 16

      onValueModified: {
        engineModel.stop();

        playStop.isEngaged = false;
        padModel.padSize = value;
        // console.log( padModel.padSize );
        padModel.newGrid();
      }
    }

    /* clear */
    Button {
      text: "Clear"
      onPressed: {
        engineModel.stop();

        playStop.isEngaged = false;
        padModel.clear();
      }
    }
  }

  /* side controls */
  Column {
    anchors.left: padGrid.right
    anchors.verticalCenter: padGrid.verticalCenter
    ComboBox {
      model: ListModel {
        ListElement { text: "C" }
        ListElement { text: "Db" }
        ListElement { text: "D" }
        ListElement { text: "Eb" }
        ListElement { text: "E" }
        ListElement { text: "F" }
        ListElement { text: "Gb" }
        ListElement { text: "G" }
        ListElement { text: "Ab" }
        ListElement { text: "A" }
        ListElement { text: "Bb" }
        ListElement { text: "B" }
      }
      onActivated: {
        var wasRunning = engineModel.isRunning();
        if ( wasRunning )
        {
          engineModel.stop();
        }

        padModel.rootNote = currentValue;

        if ( wasRunning )
        {
          engineModel.play( false );
        }
      }
    }
    ComboBox {
      model: ListModel {
        ListElement { text: "Maj7" }
        ListElement { text: "Maj9" }
        ListElement { text: "7" }
        ListElement { text: "9" }
        ListElement { text: "-7" }
        ListElement { text: "+7" }
        ListElement { text: "half dim" }
        ListElement { text: "O" }
        ListElement { text: "minMaj" }
        ListElement { text: "Maj7#5" }
        ListElement { text: "7#11" }
        ListElement { text: "7#9" }
        ListElement { text: "7#5#9" }
        ListElement { text: "7b9" }
        ListElement { text: "-9" }
        ListElement { text: "-7b9" }
        ListElement { text: "min" }
      }
      onActivated: {
        var wasRunning = engineModel.isRunning();
        if ( wasRunning )
        {
          engineModel.stop();
        }

        padModel.quality = currentValue;

        if ( wasRunning )
        {
          engineModel.play( false );
        }
      }
    }
    Button {
      text: "Save as midi"
      onClicked: fileDialog.open()
    }
  }

  /* pads */
  GridView {
    /* data */
    id: padGrid
    model: padModel

    required model

    /* anchors */
    anchors.centerIn: parent

    /* visual */
    height: Math.min( page.width, page.height  ) * 0.75
    width: height
    cellHeight: height / padModel.padSize
    cellWidth: cellHeight
    interactive: false
    Layout.minimumHeight: 700

    delegate: Component {
      Item {
        id: cont
        height: padGrid.cellHeight
        width: height

        // Rectangle {
        // anchors.fill: parent
        // color: "#220000ff"
        // }

        Button {
          // required property int index
          // required property bool engaged
          // required property bool playing

          /* data */
          id: delegate
          highlighted: engaged

          /* anchors */
          anchors.centerIn: parent
          anchors.fill: parent
          background.anchors.centerIn: delegate

          /* make actual square */
          background.height: padGrid.cellHeight - 2
          background.width: padGrid.cellHeight - 12

          /* visual */

          onPressed: {
            padModel.toggleEngaged( index );

            // background.height = background.width
            // background.width = padGrid.cellHeight - 12
            // console.log( background.width + " " + background.height );

            // padBlink.duration = engineModel.beatDuration()
            // console.log( "*** clicked "  + index + " " + engaged + " " + playing );
          }

          Rectangle {
            // required property bool playing
            // required property bool engaged

            /* data */
            id: playingRect
            anchors.centerIn: parent
            visible: playing && engaged

            /* visual */
            height: delegate.height
            width: delegate.height
            radius: 50
            color: "#00000000"

            RadialGradient {
              id: padGradient
              anchors.fill: parent
              gradient: Gradient {
                GradientStop { position: 0.0; color: "#ffffffff" }
                GradientStop { position: 0.5; color: "#00ffffff" }
              }
            }

            // states: State {
            // when: playing
            // PropertyChanges { target: playingRect; opacity: 0 }
            // }

            // transitions: Transition {
            // id: padBlink
            // PropertyAnimation {
            // properties: "opacity";
            // easing.type: Easing.InCubic
            // }
            // }
          } // Rectangle
        } // Button
      } // Item
    } // delegate
  } // GridView

  /* bottom controls */
  Row {
    anchors.top: padGrid.bottom
    anchors.horizontalCenter: padGrid.horizontalCenter
    Button {
      text: "Random"
      onClicked: randomOptionsBox.visible = ! randomOptionsBox.visible
    }
  }

  /* Random Options Box */
  Rectangle {
    id: randomOptionsBox
    visible: false
    width: 250
    height: 150
    x: page.width / 2 - width / 2
    y: page.height / 2 - height / 2
    color: "#ff303030"

    DragHandler { }

    Column {
      /* Pattern */
      anchors.fill: parent
      padding: 5
      topPadding: 10

      Label {
        text: "Notes per column pattern (e.g. 1212)"
        anchors.horizontalCenter: parent.horizontalCenter
        wrapMode: Label.WordWrap
        width: 200
      }

      TextField {
        id: randomPattern
        anchors.horizontalCenter: parent.horizontalCenter
        width: 175
        text: "1212"
        horizontalAlignment: Qt.AlignHCenter
      }

      /* Apply/Close */
      Row {
        anchors.horizontalCenter: parent.horizontalCenter
        padding: 5
        Button {
          text: "Apply"

          onClicked: {
            var wasRunning = engineModel.isRunning();
            if ( wasRunning )
            {
              engineModel.stop();
            }

            padModel.random( randomPattern.text );

            if ( wasRunning )
            {
              engineModel.play( false );
            }
          }
        }
        Button {
          text: "Close"

          onPressed: randomOptionsBox.visible = false
        }
      }
    }
  }

  DropShadow {
    id: randomOptionsBoxShadow
    visible: randomOptionsBox.visible
    anchors.fill: randomOptionsBox
    horizontalOffset: 3
    verticalOffset: 3
    spread: 0.0
    radius: 20.0
    samples: 25
    color: "#80000000"
    source: randomOptionsBox
  }

  FileDialog {
    id: fileDialog
    title: "Please choose a file"
    // folder: shortcuts.home
    folder: StandardPaths.writableLocation( StandardPaths.HomeLocation )
    fileMode: FileDialog.SaveFile
    visible: false
    onAccepted: {
      padModel.saveMidiFile( fileDialog.file );
      visible = false;
    }
    onRejected: {
      visible = false;
    }
  }
}
