#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

pthread_mutex_t moo;

struct thread_data 
{
	string in;
	string out;
};

int counter;

void *foo(void *threadarg) 
{
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	ofstream all;
	ifstream fs;
	ofstream ft;
	char ch;
	pthread_mutex_lock(&moo);
	counter+=1;
	all.open("/Lab6/log.txt",ios::app);
	all<<"/*"<<endl;
	all<<"The Thread with id: "<<counter<<" copied file from: "
	<<my_data->in<<" to: "<<my_data->out<<endl;
	all<<"*/"<<endl<<endl;
	fs.open(my_data->in);
	ft.open(my_data->out);
	if (!fs ||!ft) 
	{
		cout<<"Error!"<<endl;
		exit (123);
	}	
	while (fs.eof()==0) 
	{
		fs>>ch;
		ft<<ch;
	}
	fs.close();
	ft.close();
	all.close();
	pthread_mutex_unlock(&moo);
	pthread_exit(NULL);
	delete [] my_data;
}

int main() 
{
	const char *fullobh;
	int NUM_THREADS=0;
	int i=0;
       	int call;
	void *status;
	string check;
	string obh;
	string sender;
	string recipient;
	cout<<"Enter the name of the sender folder: ";
	sender="/Lab6/test_1";
//	cin>>sender;
	cout<<"Enter the name of the recipient folder: ";
	recipient="/Lab6/test_2";
//	cin>>recipient;
	obh="ls "+sender+">list.txt";
	fullobh=obh.c_str();
	system (fullobh);
	ifstream r1 ("/Lab6/list.txt");
	cout<<"\tThe list of files in directory: "<<sender<<" is:"<<endl;
	cout<<"\t*****************************************"<<endl;
	while (getline(r1, check)) 
	{
		cout<<"\t*\t\t"<<check<<"\t\t\t*"<<endl;
		sleep(1);
		++NUM_THREADS;
	}
	r1.close();
	cout<<"\t*****************************************"<<endl;
	cout<<"\tNum of files in directory "<<sender<<" is: " <<NUM_THREADS<<endl;
	pthread_t threads[NUM_THREADS];
	struct thread_data td[NUM_THREADS];
	string filenames[NUM_THREADS];
	ifstream r2 ("/Lab6/list.txt");
	for (i=0;i<NUM_THREADS;++i) 
	{
		getline(r2, filenames[i]);
	}
	r2.close();
	pthread_mutex_init(&moo,NULL);
	for(i=0; i<NUM_THREADS;++i) 
	{
		td[i].in=sender+"/"+filenames[i];
		td[i].out=recipient+"/"+filenames[i];
		call = pthread_create(&threads[i],NULL,foo,(void *)&td[i]);
		if (call) 
		{
			cout<<"Error: unable to create thread" <<call<<endl;
			exit(-1);
		}
	}
	for (i=0;i<NUM_THREADS;++i) 
	{
		call = pthread_join(threads[i],&status);
		if (call) 
		{
			cout<<"Error: unable to join"<<call<<endl;
		}
	}
	pthread_mutex_destroy(&moo);
	pthread_exit(NULL);
	delete [] &threads;
	delete [] &filenames;
	delete [] &td;
}
