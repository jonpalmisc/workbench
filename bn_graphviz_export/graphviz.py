# Better Graphviz Export

from binaryninja import (
    AnyFunctionType,
    BasicBlock,
    Function,
    execute_on_main_thread,
)
from binaryninjaui import getMonospaceFont

from PySide6.QtWidgets import (
    QApplication,
    QComboBox,
    QDialog,
    QGridLayout,
    QHBoxLayout,
    QLabel,
    QPlainTextEdit,
    QPushButton,
    QSpinBox,
    QVBoxLayout,
)


def block_node_name(block: BasicBlock) -> str:
    if block.is_low_level_il:
        kind = "llil"
    elif block.is_medium_level_il:
        kind = "mlil"
    elif block.is_high_level_il:
        kind = "hlil"
    else:
        kind = "asm"

    return f"{kind}_{block.index}"


def label_escape(line: str) -> str:
    return line.replace("\\n", "\\\\n").replace('"', '\\"')


def function_to_dot(
    function: AnyFunctionType, font_name: str = "Courier", font_size: int = 10
) -> str:
    NODE_ATTRIBUTES = f'shape=box fontname="{font_name}" fontsize={font_size}'

    nodes = []
    edges = []

    for block in function.basic_blocks:
        lines = [
            f"{line.address:08x}:  {line}" for line in block.get_disassembly_text()
        ]
        label = label_escape("\\l".join(lines)) + "\\l"
        nodes.append(f'  {block_node_name(block)}[{NODE_ATTRIBUTES} label="{label}"];')

    for block in function.basic_blocks:
        for edge in block.outgoing_edges:
            edges.append(
                f"  {block_node_name(block)} -> {block_node_name(edge.target)};"
            )

    return "\n".join(["digraph {"] + edges + nodes + ["}"])


class ExportDialog(QDialog):
    function: Function

    styleDropdown: QComboBox
    fontDropdown: QComboBox
    fontSizeSpinner: QSpinBox

    codeArea: QPlainTextEdit

    cancelButton: QPushButton
    copyButton: QPushButton

    rootLayout: QVBoxLayout
    optionsLayout: QGridLayout
    buttonLayout: QHBoxLayout

    def __init__(self, function: Function):
        super().__init__()

        self.function = function

        self.styleDropdown = QComboBox()
        self.styleDropdown.addItem("Disassembly")
        self.styleDropdown.addItem("LLIL")
        self.styleDropdown.addItem("MLIL")
        self.styleDropdown.addItem("HLIL")

        self.fontDropdown = QComboBox()
        self.fontDropdown.setEditable(True)
        self.fontDropdown.addItem(getMonospaceFont(self).family())
        self.fontDropdown.addItem("Courier")

        self.fontSizeSpinner = QSpinBox()
        self.fontSizeSpinner.setMinimum(8)
        self.fontSizeSpinner.setMaximum(40)
        self.fontSizeSpinner.setValue(10)

        self.codeArea = QPlainTextEdit()
        self.codeArea.setReadOnly(True)
        self.codeArea.setLineWrapMode(QPlainTextEdit.LineWrapMode.NoWrap)
        self.codeArea.setFont(getMonospaceFont(self))

        self.cancelButton = QPushButton("Cancel")
        self.copyButton = QPushButton("Copy")
        self.copyButton.setDefault(True)

        self.optionsLayout = QGridLayout()
        self.optionsLayout.addWidget(QLabel("Style"), 0, 0)
        self.optionsLayout.addWidget(self.styleDropdown, 1, 0)
        self.optionsLayout.addWidget(QLabel("Font"), 0, 1)
        self.optionsLayout.addWidget(self.fontDropdown, 1, 1)
        self.optionsLayout.addWidget(QLabel("Size"), 0, 2)
        self.optionsLayout.addWidget(self.fontSizeSpinner, 1, 2)

        self.buttonLayout = QHBoxLayout()
        self.buttonLayout.addStretch(1)
        self.buttonLayout.addWidget(self.cancelButton)
        self.buttonLayout.addWidget(self.copyButton)

        self.rootLayout = QVBoxLayout()
        self.rootLayout.addLayout(self.optionsLayout)
        self.rootLayout.addWidget(self.codeArea)
        self.rootLayout.addLayout(self.buttonLayout)

        self.setWindowTitle("Graphviz Export")
        self.setLayout(self.rootLayout)
        self.setMinimumSize(600, 500)

        self.styleDropdown.currentIndexChanged.connect(self.refreshOutput)
        self.fontDropdown.currentTextChanged.connect(self.refreshOutput)
        self.fontSizeSpinner.valueChanged.connect(self.refreshOutput)
        self.cancelButton.clicked.connect(self.reject)
        self.copyButton.clicked.connect(self.copyAndAccept)

        self.refreshOutput()

    def selectedFunction(self) -> AnyFunctionType:
        styleName = self.styleDropdown.currentText()
        if styleName == "LLIL":
            return self.function.llil
        elif styleName == "MLIL":
            return self.function.mlil
        elif styleName == "HLIL":
            return self.function.hlil
        else:
            return self.function

    def selectedFontName(self) -> str:
        return self.fontDropdown.currentText()

    def selectedFontSize(self) -> int:
        return self.fontSizeSpinner.value()

    def copyAndAccept(self):
        clipboard = QApplication.clipboard()
        clipboard.clear()
        clipboard.setText(self.codeArea.toPlainText())

        self.accept()

    def refreshOutput(self):
        self.codeArea.setPlainText(
            function_to_dot(
                self.selectedFunction(),
                self.selectedFontName(),
                self.selectedFontSize(),
            )
        )


def do_export():
    dialog = ExportDialog(current_function)
    dialog.exec()


execute_on_main_thread(do_export)
