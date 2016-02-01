/**
* Autogenerated Source File for TEvioFileReader
* \\file TEvioFileReader.cc
* \\author G.Gavalian
* \\date 2012-07-14
*/
#include "EvioFileReader.h"
//#include "TEvioDataBank.h"

#include "evio.h"
#include "evioUtil.hxx"
#include "evioBankIndex.hxx"
#include "unistd.h"
#include "errno.h"
EvioFileReader::EvioFileReader()
{
  cout << "***************************************************************" << endl;
  cout << "*  Initializing EvioFileReader library implementing interface *" << endl;
  cout << "***************************************************************" << endl;
  fileEventsCount = 0;
}
/*
TEvioFileReader::TEvioFileReader(const TEvioFileReader &obj)
{
}
*/

EvioFileReader::~EvioFileReader()
{
}

void EvioFileReader::close()
{
  evClose(evioFileHandle);
  cout << " EVENTS READ = " << numberOfEventsRead << "  TIME = " 
          << readTimeTotalMS
          << " ms " << endl;
}
/**
 * Open a File with given filename. Check for access.
 */
void EvioFileReader::open(const char *filename)
{
  char file[128];
  currentFileName = filename;
  sprintf(file,"%s",filename);
  uint32_t *header=new uint32_t[8];

  int access_status = access(file,R_OK);
  if(access_status!=0){
    cout << endl << "E R R O R : accessing file " 
        << file << ". error code = " << access_status << endl;


        char buffer[256];
        strerror_r( errno, buffer, 256 ); // get string message from errno
        printf("Error: %i - %s \n",errno,buffer);

  }
  cout<<"Going to call evOpen now"<<endl;
  evOpen(file,"ra",&evioFileHandle);
  evIoctl(evioFileHandle, "E", (void *)&fileEventsCount);
 /* evIoctl(evioFileHandle, "V", (void *)&version);*/
/*  evIoctl(evioFileHandle, "H", (void *)&header);*/
  numberOfEventsRead = 0;
  readTimeTotalMS    = 0.0;
  cout << "***** OPENED FILE : " << filename << "  NEVENT = " << fileEventsCount <<" VERSION = "<<version<<endl;

  for (int ii=0;ii<8;ii++) printf("%x ",header[ii]);
  cout << endl;

  uint32_t evCount, len = 0L, bufLen;
  int i,status;
  const uint32_t **pTable;

/*
const uint32_t *evioptr;
  uint32_t        bufferlen = 0;
  int read_status = evReadRandom(evioFileHandle,&evioptr,&bufferlen,6);
dataEvent.init(evioptr,bufferlen);
*/
/*
  status = evGetRandomAccessTable(evioFileHandle, &pTable, &len);
    printf("  i      pointers\n");
    printf("-------------------\n");
    for (i=0; i < len; i++) {
        printf("  %d    %p\n", i, pTable[i]);
  }*/
  printf("EvioFileReader: file opened!\n");
}

int   EvioFileReader::getEntries()
{
  return fileEventsCount;
}
/**
 * Reads next event into internal buffer. returns true if there is an event in the file,
 * and false if end of file is reached.
 */

bool EvioFileReader::next()
{
    long double start_time = time(0);
    int read_status = evRead(evioFileHandle,buffer,MAXEVIOBUF);
    //cout << "read status = " << read_status << endl;
    long double end_time   = time(0);
    //cout << " measured = " << start_time << " " << end_time << endl;
    if(read_status!=-1){
        readTimeTotalMS += (end_time-start_time)*1000.0;
        numberOfEventsRead += 1;
        dataEvent.init(buffer,MAXEVIOBUF);
        return true;
    }
    return false;
}

/**
* Read event with given number
*/
int   EvioFileReader::readEvent(int eventnum)
{
  const uint32_t *evioptr;
  uint32_t        bufferlen = 0;
  int read_status = evReadRandom(evioFileHandle,&evioptr,&bufferlen,eventnum+1);
  //cout << "\033[1;34m Read Event # " << eventnum << "  length = " << bufferlen
  //<< "  status = " << read_status
  //<<  "\033[0;0m" << endl;

  dataEvent.init(evioptr,bufferlen);
  
  //dataEvent.init(buffer,bufferlen);
}
/**
* Returns Event Object 
*/
EvioDataEvent &EvioFileReader::getEvent()
{
    return dataEvent;
}

void  EvioFileReader::getEvent(EvioDataEvent &event)
{
    event.init(buffer,MAXEVIOBUF);
}

vector<double>   *EvioFileReader::getDoubleVector(int tag, int num)
{
  evio::evioBankIndex b0(buffer);
  evio::bankIndex b;
  if(b0.tagNumExists(evio::tagNum(tag,num))){
    //cout << " found data with tag and num = " << tag << "  " << num << endl;                                                                           
    b = b0.getBankIndex(evio::tagNum(tag,num));
    int dataLength;
    const double *data_ptr = b0.getData<double>(b,&dataLength);
    //cout << " found data with tag and num = " << tag << "  " << num << "  length = " << dataLength << endl;                                            
    vector<double> *vec_ptr = new vector<double>(dataLength);
    memcpy(vec_ptr->data(),data_ptr,dataLength*sizeof(double));
    return vec_ptr;
  }
  return new vector<double>();
}

vector<int32_t>  *EvioFileReader::getIntVector(int tag, int num)
{
  evio::evioBankIndex b0(buffer);
  evio::bankIndex b;
  
  if(b0.tagNumExists(evio::tagNum(tag,num))){
    //cout << " found data with tag and num = " << tag << "  " << num << endl;
    b = b0.getBankIndex(evio::tagNum(tag,num));
    int dataLength;
    const int32_t *data_ptr = b0.getData<int32_t>(b,&dataLength);
    //cout << " found data with tag and num = " << tag << "  " << num << "  length = " << dataLength << endl;
    vector<int32_t> *vec_ptr = new vector<int32_t>(dataLength);
    memcpy(vec_ptr->data(),data_ptr,dataLength*sizeof(int32_t));
    return vec_ptr;
  }
  return new vector<int32_t>();
}

void              EvioFileReader::getBank(EvioDataBank& bank,int tag, int num)
{
  
}

string            EvioFileReader::getDataFormat(int tag, int num)
{
}

