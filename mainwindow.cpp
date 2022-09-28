#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "myjgs.h"
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _dragger = new DraggerWidget(ui->widgetdrag);

    connect(_dragger, &DraggerWidget::sthDragged, this, [&](const DraggerWidget::DragObjects_t& d) {
        if (d.type != DraggerWidget::Type::TYPE_FILE) {
            return;
        }

        if (d.file_list.size() < 1) {
            return;
        }

        auto file = d.file_list.at(0);

        try {
            myjgs::Game::ptr game = std::make_shared<myjgs::Game>(file.toStdString());
            game->process_all_events();
            ui->textBrowser->clear();

            QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
            QTextCodec *gbk = QTextCodec::codecForName("gb2312");
            QTextCodec::setCodecForLocale(utf8);


            for (uint8_t i = 0; i < 4; ++i) {
                myjgs::PlayerColor color = (myjgs::PlayerColor)i;

                ui->textBrowser->append("color:" + QString(myjgs::itemcolor_type2string(color).c_str()));
                ui->textBrowser->append("qq:" + QString::number(game->get_player(color).qq()));
                QString name_unicode = gbk->toUnicode(game->get_player(color).name());
                QByteArray name_utf8 = utf8->fromUnicode(name_unicode);
//                qDebug() << name_utf8;
                ui->textBrowser->append("name:" + name_utf8);
                ui->textBrowser->append("remain socre:" + QString::number(game->get_player(color).get_current_score()) + "\n");

            }

        } catch (const myjgs::GameException& ge) {
            ui->textBrowser->clear();
            QString tmp{"GameException"};
            tmp.append(ge.what());
            ui->textBrowser->setText(tmp);
        } catch (const std::exception& stde) {
            ui->textBrowser->clear();
            QString tmp{"std::exception"};
            tmp.append(stde.what());
            ui->textBrowser->setText(tmp);
        } catch (...) {
            ui->textBrowser->clear();
            ui->textBrowser->setText("unknown error");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

