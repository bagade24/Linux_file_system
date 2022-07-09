#include<iostream>
#include<stdio.h>
#include<bits/stdc++.h>
#include<string>
#include <unistd.h>
#include <sys/types.h>
#define ll long long
using namespace std;

class FileStructure{
public:
	ll filedescriptor;
	ll offset;
	ll blockno;
	ll sizee;
	
	FileStructure()
	{
		
	}
};

class File{
	public:
	string name;
	bool append,read,write;
	File()
	{

	}
};

class Disk{
public:
///////////////////////////////
string diskname;
ll blocksize=1000000;
ll numberofblocks;
vector<FileStructure> entry;
string modes[3]={"read","write","append"};
string test="created";
///////////////////////////////
map<string,File> filedetails;
map<int,string> filedescriptors;
ll createcount=0;
ll blockno=0;
///////////////////////////////
Disk()
{

}

Disk(string diskname){

   this->diskname=diskname;
   FILE* disknamefp = fopen(diskname.c_str(), "wb");
   ftruncate(fileno(disknamefp), 5*1000000);
   numberofblocks=5*1000000/blocksize;
   cout<<"[+] DISK CREATED ::500MB::"<<numberofblocks<<"\n";
   fclose(disknamefp);
   
   for(ll i=0;i<20;i++)
   {
   	FileStructure fs;
   	entry.push_back(fs);
   }

  /* ofstream file;
   file.open(diskname.c_str(), ios::app|ios::binary);
   file.write((char*)this,sizeof(*this));
   file.close();
	*/

  }

 void mountdisk()
 {	
 	//ifstream file2;
 	/*file2.open(diskname.c_str(),ios::in|ios::binary);
   	file2.seekg(0);
   	file2.read((char*)this,sizeof(*this));
 	cout<<this->test<<"\n";
	*/

 	cout<<"[+] Mounted successfully "<<diskname<<"\n";

 	while(1)
 	{
 	cout<<"1.create file\n";
 	cout<<"2. open file:\n";
 	cout<<"3. read file:\n";
 	cout<<"4. write file:\n";
 	cout<<"5. append file:\n";
 	cout<<"6. close file:\n";
 	cout<<"7. delete file:\n";
 	cout<<"8. list of files:\n";
 	cout<<"9. list of opened files:\n";
 	cout<<"10. unmount:\n";
 	ll choice;
 	cin>>choice;

 	if(choice==1) createfile();

 	if(choice==2) openfile();

 	if(choice==3) readfile();

 	if(choice==4) writefile();

 	if(choice==5) appendfile();

 	if(choice==6) closefile();

 	if(choice==7) deletefile();

 	if(choice==8) listoffiles();

 	if(choice==9) listofopenedfiles();

 	if(choice==10){
 		unmount();
 		return;
 	}

 	}

 	
 }

 void unmount()
 {
 	ofstream file;
   	file.open(diskname.c_str(), ios::in|ios::binary|ios::ate);
   	file.seekp(blocksize*numberofblocks-1);
   	file.write((char*)this,sizeof(*this));
   	file.close();	
 }

 void createfile()
 {
	
 	string filename;
 	cin>>filename;
	File newfile;
	newfile.name=filename;
 	filedetails[filename]=newfile;
 	cout<<"[+]"<<filename<<" File created successfully\n";

	entry[createcount].filedescriptor=createcount;
	entry[createcount].sizee=0;
	entry[createcount].blockno=blockno;
	filedescriptors[createcount]=filename;

	createcount++;
	blockno++;
 }

  void openfile()
 {
 	
	
 	cout<<"[] Enter file to be opened\n";
 	string filename;
 	cin>>filename;
 	if(filedetails.find(filename)==filedetails.end())
	{
		cout<<"[-] File Doesn't exit\n";
		return;
	}
 	ll modech;
 	cout<<"0:read 1: write 2:append\n";
 	cin>>modech;
	if(modech==0) filedetails[filename].read=1;

	if(modech==1) filedetails[filename].write=1;

	if(modech==2) filedetails[filename].append=1;


 	cout<<"[+] File opened in "<<modes[modech]<<" mode\n";

 	for(auto x: filedescriptors)
 	{
 		if(x.second==filename)
 		{
 			cout<<"[+] File descriptor for file is generated :"<<x.first<<"\n";
 			break;
 		}
 	}

 }

  void readfile()
 {
	 cout<<"[] Enter file descriptor\n";
 	int inp;
 	cin>>inp;
 	string filename=filedescriptors[inp];
	
 	if(filedetails.find(filename)==filedetails.end())
	{
		cout<<"[-] File Doesn't exit\n";
		return;
	}
	if(!filedetails[filename].read)
	{
		cout<<"[-] File not opened in write mode\n";
		return;
	}

	ll offset=entry[inp].offset;
	ll filesize=entry[inp].sizee;
	ll bytes=offset+filesize;

	cout<<offset<<"XX\n";

		ifstream file(diskname.c_str(),ios::in);

		cout<<file.tellg()<<"CC "<<offset<<"\n";
		file.seekg(offset,ios::beg);
		cout<<filesize<<"CC\n";
		char s[filesize+1];
		file.read(s, filesize);
		s[filesize]='\0';
		file.close();
		cout<<"Rading from file..\n"<<s<<"\n";
		
 }

  void writefile()
 {
	cout<<"[] Enter file descriptor\n";
 	int inp;
 	cin>>inp;
 	string filename=filedescriptors[inp];
	
 	if(filedetails.find(filename)==filedetails.end())
	{
		cout<<"[-] File Doesn't exit\n";
		return;
	}
	if(!filedetails[filename].write)
	{
		cout<<"[-] File not opened in write mode\n";
		return;
	}

	cout<<"[+] Enter data to write in file\n";
	string mydata;
	std::getline(std::cin >> std::ws, mydata);  
	ll sizeoffile=mydata.size();
	if(sizeoffile >blocksize)
	{
		cout<<"[-] File too big\n";
		return;
	}

	ll startoffset=entry[inp].blockno*blocksize;
	
	ofstream MyFile(diskname,ios::binary|ios::ate|ios::in);  

	cout<<MyFile.cur<<"CC "<<startoffset<<"\n";
	MyFile.seekp(startoffset);
  	MyFile.write(mydata.data(),mydata.size());

	entry[inp].sizee=sizeoffile;
	entry[inp].offset=startoffset;
	//cout<<startoffset<<"YY\n";
	MyFile.close();
	cout<<"[+] File written successfully\n"; 
	return;
 }

  void appendfile()
 {
 	cout<<"[] Enter file descriptor\n";
 	int inp;
 	cin>>inp;
 	string filename=filedescriptors[inp];
	
 	if(filedetails.find(filename)==filedetails.end())
	{
		cout<<"[-] File Doesn't exit\n";
		return;
	}
	if(!filedetails[filename].append)
	{
		cout<<"[-] File not opened in write mode\n";
		return;
	}

	cout<<"[+] Enter data to write in file\n";
	string mydata;
	std::getline(std::cin >> std::ws, mydata);

	ll sizeoffile=mydata.size();
	if(sizeoffile >blocksize)
	{
		cout<<"[-] File too big\n";
		return;
	}

	ll startoffset=entry[inp].blockno*blocksize+entry[inp].sizee;
	
	ofstream MyFile(diskname,ios::binary|ios::ate|ios::in);

	cout<<MyFile.cur<<"CC "<<startoffset<<"\n";
	MyFile.seekp(startoffset);
  	MyFile.write(mydata.data(),mydata.size());

  	//cout<<entry[inp].sizee<<"YY"<<sizeoffile;
	entry[inp].sizee=entry[inp].sizee+sizeoffile;
	//entry[inp].offset=startoffset;
	//cout<<startoffset<<"YY\n";
	MyFile.close();
	cout<<"[+] File appended successfully\n"; 
	return;
 }

  void closefile()
 {
 	cout<<"[] Enter file descriptor\n";
 	int inp;
 	cin>>inp;
 	string filename=filedescriptors[inp];
	
 	if(filedetails.find(filename)==filedetails.end())
	{
		cout<<"[-] File Doesn't exit\n";
		return;
	}
 	filedetails[filename].read=0;
	filedetails[filename].write=0;
	filedetails[filename].append=0;
 }

  void deletefile()
 {
 	string filename;
 	cin>>filename;
 	//files[filename]="created";
	filedetails.erase(filename);

 }

  void listoffiles()
 {
 	for(auto x:filedetails)
 	{
 		cout<<x.first<<"\n";
 	}
 }

  void listofopenedfiles()
 {
 	for(auto x:entry)
 	{
 		for(auto y:filedescriptors)
		 {
			 if(x.filedescriptor==y.first)
			 {
				 cout<<"[] "<<y.second<<"\n";
			 }
		 }
 	}
 }

};


int main()
{
	map<string,Disk> myprogram;    
    
    ll choice=1;

    while(choice)
    {
    	cout<<"1: create disk\n2: mount disk \n3: exit\n";
    	
    	cin>>choice;
    	if(choice==1)
    	{
    		string inp;
    		cin>>inp;
    		Disk disk(inp+".txt");
    		myprogram.insert({inp,disk});
    	}

    	if(choice==2)
    	{
    		string inp;
    		cin>>inp;
    		if(myprogram.find(inp)==myprogram.end())
    		{
    			myprogram[inp].mountdisk();   
    		}
    		else{
    			 
				 ifstream file(inp,ios::in|ios::ate);
				 ll offset=(myprogram[inp].numberofblocks-1)*(myprogram[inp].blocksize);				
				 file.seekg(offset,ios::beg);
				 Disk obj;
				 file.open(inp+".txt", ios::in);
									
				file.read((char*)&obj, sizeof(obj));
				obj.mountdisk();

    		}
    		 		
    	}

    	if(choice==3)
    	{
    		choice=0;
    		exit(0);	
    	}

    }
    
    return 0;
}