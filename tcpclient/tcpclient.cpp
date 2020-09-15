#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>

TcpClient::TcpClient(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	setWindowTitle(tr("TCP Client"));//设置窗体的标题

									 //初始化各个控件
	contentListWidget = new QListWidget;
	sendLineEdit = new QLineEdit;
	sendBtn = new QPushButton(QStringLiteral("发送"));
	userNameLabel = new QLabel(QStringLiteral("用户名："));
	userNameLineEdit = new QLineEdit;
	serverIPLabel = new QLabel(QStringLiteral("服务器地址："));
	serverIPLineEdit = new QLineEdit;
	portLabel = new QLabel(QStringLiteral("端口："));
	portLineEdit = new QLineEdit;
	enterBtn = new QPushButton(QStringLiteral("进入聊天室"));

	//设置布局
	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(contentListWidget, 0, 0, 1, 2);
	mainLayout->addWidget(sendLineEdit, 1, 0);
	mainLayout->addWidget(sendBtn, 1, 1);
	mainLayout->addWidget(userNameLabel, 2, 0);
	mainLayout->addWidget(userNameLineEdit, 2, 1);
	mainLayout->addWidget(serverIPLabel, 3, 0);
	mainLayout->addWidget(serverIPLineEdit, 3, 1);
	mainLayout->addWidget(portLabel, 4, 0);
	mainLayout->addWidget(portLineEdit, 4, 1);
	mainLayout->addWidget(enterBtn, 5, 0, 1, 2);

	status = false;

	//初始化TCP端口号
	port = 8010;
	portLineEdit->setText(QString::number(port));

	serverIP = new QHostAddress();

	connect(enterBtn, SIGNAL(clicked()), this, SLOT(slotEnter()));
	connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotSend()));

	sendBtn->setEnabled(false);
}

TcpClient::~TcpClient()
{

}

void TcpClient::slotEnter()
{
	//status表示当前的状态
	if (!status)
	{
		//完成输入合法性检验
		QString ip = serverIPLineEdit->text();
		if (!serverIP->setAddress(ip)) //判断给定的IP是否能够被正确解析
		{
			QMessageBox::information(this, tr("error"), tr("server ip address error!"));
			return;
		}

		if (userNameLineEdit->text() == "")
		{
			QMessageBox::information(this, tr("error"), tr("User name error!"));
			return;
		}

		userName = userNameLineEdit->text();

		//初始化TCP客户端：创建一个QTcpSocket类对象，并将信号/槽连接起来
		tcpSocket = new QTcpSocket(this); //实例化tcpSocket
		tcpSocket->abort(); //取消原有连接
		connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
		connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
		connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

		//与TCP服务器连接，连接成功后发出conneted()信号
		tcpSocket->connectToHost(*serverIP, port);

		status = true;
	}
	else
	{
		int length = 0;
		QString msg = userName + tr(":Leave Chat Room"); //构建一条离开聊天室的消息
														 //通知服务器端以上构建的信息
		if ((length = tcpSocket->write(msg.toLatin1(), msg.length())) != msg.length())
		{
			return;
		}

		//与服务器断开连接，断开连接后发出disconnected()信号
		tcpSocket->disconnectFromHost();

		status = false;
	}
}


/*******************************
* slotConnected()为connected()信号的响应槽，
* 当与服务器连接成功后，客户端构造一条进入聊天室的信息，并通知服务器
*******************************/
void TcpClient::slotConnected()
{
	sendBtn->setEnabled(true);
	enterBtn->setText(QStringLiteral("离开"));

	int length = 0;
	QString msg = userName + tr(":Enter Chat Room");
	if ((length = tcpSocket->write(msg.toLatin1(), msg.length())) != msg.length())
	{
		return;
	}
}

void TcpClient::slotSend()
{
	if (sendLineEdit->text() == "")
	{
		return;
	}

	QString msg = userName + ":" + sendLineEdit->text();

	//write()发送数据
	tcpSocket->write(msg.toLatin1(), msg.length());
	sendLineEdit->clear();
}

void TcpClient::slotDisconnected()
{
	sendBtn->setEnabled(false);
	enterBtn->setText(QStringLiteral("进入聊天室"));
}

/*******************************
* dataReceived()函数，当有数据到来时，触发此函数，
* 从套接字中将有效数据取出并显示
*******************************/
void TcpClient::dataReceived()
{
	/*bytesAvailable()表示有效数据*/
	while (tcpSocket->bytesAvailable()>0)
	{
		QByteArray datagram;
		datagram.resize(tcpSocket->bytesAvailable());

		//read()读数据
		tcpSocket->read(datagram.data(), datagram.size());

		QString msg = datagram.data();
		contentListWidget->addItem(msg.left(datagram.size()));
	}
}