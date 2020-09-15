#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>

TcpClient::TcpClient(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	setWindowTitle(tr("TCP Client"));//���ô���ı���

									 //��ʼ�������ؼ�
	contentListWidget = new QListWidget;
	sendLineEdit = new QLineEdit;
	sendBtn = new QPushButton(QStringLiteral("����"));
	userNameLabel = new QLabel(QStringLiteral("�û�����"));
	userNameLineEdit = new QLineEdit;
	serverIPLabel = new QLabel(QStringLiteral("��������ַ��"));
	serverIPLineEdit = new QLineEdit;
	portLabel = new QLabel(QStringLiteral("�˿ڣ�"));
	portLineEdit = new QLineEdit;
	enterBtn = new QPushButton(QStringLiteral("����������"));

	//���ò���
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

	//��ʼ��TCP�˿ں�
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
	//status��ʾ��ǰ��״̬
	if (!status)
	{
		//�������Ϸ��Լ���
		QString ip = serverIPLineEdit->text();
		if (!serverIP->setAddress(ip)) //�жϸ�����IP�Ƿ��ܹ�����ȷ����
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

		//��ʼ��TCP�ͻ��ˣ�����һ��QTcpSocket����󣬲����ź�/����������
		tcpSocket = new QTcpSocket(this); //ʵ����tcpSocket
		tcpSocket->abort(); //ȡ��ԭ������
		connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
		connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
		connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

		//��TCP���������ӣ����ӳɹ��󷢳�conneted()�ź�
		tcpSocket->connectToHost(*serverIP, port);

		status = true;
	}
	else
	{
		int length = 0;
		QString msg = userName + tr(":Leave Chat Room"); //����һ���뿪�����ҵ���Ϣ
														 //֪ͨ�����������Ϲ�������Ϣ
		if ((length = tcpSocket->write(msg.toLatin1(), msg.length())) != msg.length())
		{
			return;
		}

		//��������Ͽ����ӣ��Ͽ����Ӻ󷢳�disconnected()�ź�
		tcpSocket->disconnectFromHost();

		status = false;
	}
}


/*******************************
* slotConnected()Ϊconnected()�źŵ���Ӧ�ۣ�
* ������������ӳɹ��󣬿ͻ��˹���һ�����������ҵ���Ϣ����֪ͨ������
*******************************/
void TcpClient::slotConnected()
{
	sendBtn->setEnabled(true);
	enterBtn->setText(QStringLiteral("�뿪"));

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

	//write()��������
	tcpSocket->write(msg.toLatin1(), msg.length());
	sendLineEdit->clear();
}

void TcpClient::slotDisconnected()
{
	sendBtn->setEnabled(false);
	enterBtn->setText(QStringLiteral("����������"));
}

/*******************************
* dataReceived()�������������ݵ���ʱ�������˺�����
* ���׽����н���Ч����ȡ������ʾ
*******************************/
void TcpClient::dataReceived()
{
	/*bytesAvailable()��ʾ��Ч����*/
	while (tcpSocket->bytesAvailable()>0)
	{
		QByteArray datagram;
		datagram.resize(tcpSocket->bytesAvailable());

		//read()������
		tcpSocket->read(datagram.data(), datagram.size());

		QString msg = datagram.data();
		contentListWidget->addItem(msg.left(datagram.size()));
	}
}