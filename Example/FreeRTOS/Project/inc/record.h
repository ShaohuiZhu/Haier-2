#ifndef record_h
#define record_h
//�յ�����״̬��Ϣ�ṹ��
struct air_condition
{
	char id;//�豸��
	
	char month;//��
	char week;//�ܼ�
	char day;//��
	char hour;//Сʱ
	char minute;//����

	char out_temp;//�����¶�
	char in_temp;//�����¶�
	char air_temp;//�յ��¶�
	char set_temp;//�����¶�

	char out_mois;//����ʪ��
	char in_mois;//����ʪ��

	char air_windV;//�յ�����
	char set_windV;//���÷���

	char air_windUD;//�յ����·���
	char set_windUD;//���÷���

	char air_windLR;//�յ����ҷ���
	char set_windLR;//���÷���

	char air_mode;//�յ�ģʽ
	char set_mode;//����ģʽ

	char air_scene;//�յ�����
	char set_scene;//���ó���

	char air_status;//�Ƿ񿪹ػ�
	char set_status;//�Ƿ񿪹ػ�
	
	char is_person;//�Ƿ�����
	char air_fault; //��������
};
#endif // record_h