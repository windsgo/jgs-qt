#include "draggerwidget.h"
#include "ui_draggerwidget.h"

DraggerWidget::DraggerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DraggerWidget)
{
    ui->setupUi(this);
    _initWidget();
}

DraggerWidget::~DraggerWidget()
{
    delete ui;
}

void DraggerWidget::_initWidget()
{
    this->setAcceptDrops(true); // 允许拖入
    this->setWindowFlags(Qt::FramelessWindowHint); // 取消标题栏
    _ui_normal();
}

void DraggerWidget::_ui_success() {
    _green();
    ui->label->setText("拖入识别成功");
}

void DraggerWidget::_ui_fail() {
    _red();
    ui->label->setText("文件识别失败");
}

void DraggerWidget::_ui_dragging() {
    _orange();
    ui->label->setText("请松开鼠标");
}

void DraggerWidget::_ui_normal() {
    _yellow();
    ui->label->setText("请拖入");
}

void DraggerWidget::dragEnterEvent(QDragEnterEvent *event)
{
    _ui_dragging();
    auto mime_data = event->mimeData();
    if (mime_data->hasUrls()) {
        // 拖入的对象有路径
        if (mime_data->urls().size() <= 0) {
            event->ignore();
            return;
        }

        event->acceptProposedAction();
    } else {
        event->ignore();
        qDebug() << "ignore";
    }
}

void DraggerWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    _ui_normal();
}

void DraggerWidget::dropEvent(QDropEvent *event)
{
    auto mime_data = event->mimeData();
    if (mime_data->hasUrls()) {
        // 拖入的对象有路径
        if (mime_data->urls().size() <= 0) {
            event->ignore();
            _ui_fail();
            return;
        }

        _last_d.clear();
        for (const auto& url : mime_data->urls()) {
            if (!url.isValid() || url.isEmpty()) {
                continue;
            }

            auto str = url.toLocalFile();
            auto path = QFileInfo(str);
            if (path.isFile()) {
                _last_d.type = static_cast<enum Type>(_last_d.type | TYPE_FILE);
                _last_d.file_list.append(str);
            } else if (path.isDir()) {
                _last_d.type = static_cast<enum Type>(_last_d.type | TYPE_FOLDER);
                _last_d.folder_list.append(str);
            }
        }
        if (!_last_d.isEmpty()) {
            _ui_success();
            emit sthDragged(_last_d);
        }
    } else {
        event->ignore();
        _ui_fail();
    }
}


QDebug operator<<(QDebug os, const DraggerWidget::DragObjects_t &d) {
    os << d.str() << '\n';
    return os;
}
