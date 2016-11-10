//	������������ݽṹ��en_code���������ݰ����б���
// de_code���������ݰ�����

#ifndef NET_PACKET_H
#define NET_PACKET_H

#define HEADER_LENGTH 5
#define MARK_LENGTH 6
#define SERIAL_NUM_LENGTH 10
#define KEY_LENGTH 32
#define DATA_LENGTH 1024*8
#define PACKET_LENGTH (HEADER_LENGTH + MARK_LENGTH + SERIAL_NUM_LENGTH + KEY_LENGTH + DATA_LENGTH)

struct net_packet 
{
	char* c_head;										//���������,4���ֽڳ���
	char* c_mark;										//��ʶ���
	char* c_serial_num;					//�����кţ�9���ֽڳ���
	char* c_key;												//��Ψһ��ʶ
	char* c_buffer;									//�����ݻ�����

	net_packet()
	{
		c_head = new char[HEADER_LENGTH];
		c_mark = new char[MARK_LENGTH];
		c_serial_num = new char[SERIAL_NUM_LENGTH];
		c_key = new char[KEY_LENGTH];
		c_buffer = new char[PACKET_LENGTH];
		memset(c_head,'0',HEADER_LENGTH - 1);
		memset(c_serial_num,'0',SERIAL_NUM_LENGTH - 1);
		c_head[HEADER_LENGTH-1] = 0;
		c_serial_num[SERIAL_NUM_LENGTH-1] = 0;
	}
	~net_packet()
	{
		delete []c_head;
		delete []c_mark;
		delete []c_serial_num;
		delete []c_key;
		delete []c_buffer;
	}
	void cast_to_array(char* p_array, int array_length, int i)
	{
		char * p_buffer = new char[array_length];
		itoa(i,p_buffer,10);
		int real_length = strlen(p_buffer);
		int index = array_length - real_length-1;
		for(int i = 0; i < real_length; ++i , ++index)
			p_array[index] = p_buffer[i];
		delete []p_buffer;
	}


	int get_packet_length()
	{
		int packet_length = atoi(c_head);
		return packet_length;
	}

	char* data(int& length)
	{
		int packet_length = get_packet_length();
		length = packet_length - HEADER_LENGTH -MARK_LENGTH - SERIAL_NUM_LENGTH - KEY_LENGTH;
		return c_buffer + HEADER_LENGTH + MARK_LENGTH + SERIAL_NUM_LENGTH + KEY_LENGTH;
	}

	void en_code(const char* p_mark, int seral_num, 
		const char* p_key, const char* p_buffer, int len)
	{
		//������ݰ�����
		int packet_length = HEADER_LENGTH+MARK_LENGTH+SERIAL_NUM_LENGTH+KEY_LENGTH+len;
		cast_to_array(c_head,HEADER_LENGTH,packet_length);
		//���MARK
		memcpy(c_mark,p_mark,MARK_LENGTH);
		//������к�
		cast_to_array(c_serial_num,SERIAL_NUM_LENGTH,seral_num);
		//���Ψһ��ʶ
		memcpy(c_key,p_key,KEY_LENGTH);
		//�������
		memset(c_buffer,0,PACKET_LENGTH);
		memcpy(c_buffer,c_head,HEADER_LENGTH);
		memcpy(c_buffer+HEADER_LENGTH,c_mark,MARK_LENGTH);
		memcpy(c_buffer+HEADER_LENGTH+MARK_LENGTH,c_serial_num,SERIAL_NUM_LENGTH);
		memcpy(c_buffer+HEADER_LENGTH+MARK_LENGTH+SERIAL_NUM_LENGTH,c_key,KEY_LENGTH);
		memcpy(c_buffer+PACKET_LENGTH-DATA_LENGTH,p_buffer,len);
		return;
	}

	int de_code(const char* p_data, int length)
	{
		//��ȡ������,�ж�һ�����������Ƿ���ȫ
		if(length < 5)
			return 0;
		memcpy(c_head,p_data,5);
		int packet_length = atoi(c_head);
		if(length < packet_length)
			return 0;
		//�����ݸ��Ƶ���������
		memcpy(c_buffer,p_data,packet_length);
		//��������
		memcpy(c_mark,c_buffer+HEADER_LENGTH,MARK_LENGTH);
		memcpy(c_serial_num,c_buffer+HEADER_LENGTH+MARK_LENGTH,SERIAL_NUM_LENGTH);
		memcpy(c_key,c_buffer+HEADER_LENGTH+MARK_LENGTH+SERIAL_NUM_LENGTH,KEY_LENGTH);
		return packet_length;
	}
};


#endif	//NET_PACKET_H
