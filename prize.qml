import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.VirtualKeyboard
import QtQuick.Layouts

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    id: root
    width: 400
    height: 600
    visible: true
    title: "Teclado Customizado"

    // Variável que guarda qual input está recebendo texto
    property var activeInput: null

    // Clicar no fundo da tela fecha o teclado
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.activeInput = null
            root.focus = true // Tira o foco visual do TextField
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 20

        TextField {
            id: input1
            placeholderText: "Campo 1..."
            // Usamos onPressed ao invés de onActiveFocusChanged
            onPressed: root.activeInput = input1
        }

        TextField {
            id: input2
            placeholderText: "Campo 2..."
            onPressed: root.activeInput = input2
        }
    }

    // --- TECLADO ANIMADO CORRIGIDO ---
    Rectangle {
        id: keyboardContainer
        width: parent.width
        height: 250
        color: "#d1d5db"

        // Começa escondido
        y: parent.height

        states: [
            State {
                name: "visible"
                // O teclado aparece se houver um input ativo
                when: root.activeInput !== null
                PropertyChanges {
                    target: keyboardContainer
                    y: parent.height - keyboardContainer.height
                }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation { properties: "y"; duration: 300; easing.type: Easing.OutCubic }
            }
        ]

        GridLayout {
            anchors.centerIn: parent
            columns: 3
            rowSpacing: 10
            columnSpacing: 10

            Repeater {
                model: ["1", "2", "3", "4", "5", "6", "7", "8", "9"]
                Button {
                    text: modelData
                    property string btnValue: modelData

                    // A SOLUÇÃO: Isso impede o botão de roubar o cursor do TextField!
                    focusPolicy: Qt.NoFocus

                    onClicked: {
                        if (root.activeInput) {
                            root.activeInput.insert(root.activeInput.cursorPosition, btnValue)
                        }
                    }
                }
            }

// Botão Fechar
            Button {
                icon.source: "qrc:/icons/res/calcclr.png"
                icon.width: 32
                icon.height: 32
                icon.color: "transparent"
                flat: true
                display: AbstractButton.IconOnly
                text: "Fechar"
                focusPolicy: Qt.NoFocus
                onClicked: {
                    root.activeInput = null
                    root.focus = true
                }
            }

            // Botão 0
            Button {
                text: "0"
                focusPolicy: Qt.NoFocus
                onClicked: {
                    if (root.activeInput) {
                        root.activeInput.insert(root.activeInput.cursorPosition, text)
                    }
                }
            }

            // Botão Apagar (Backspace)
            Button {
                text: "⌫"
                focusPolicy: Qt.NoFocus

                // MÁGICA DE UX: Permite segurar o botão para apagar vários caracteres
                autoRepeat: true

                onClicked: {
                    if (root.activeInput && root.activeInput.cursorPosition > 0) {
                        // Pega a posição atual do cursor
                        let pos = root.activeInput.cursorPosition;
                        // Remove o caractere imediatamente anterior ao cursor
                        root.activeInput.remove(pos - 1, pos);
                    }
                }
            }
        }
    }
}
