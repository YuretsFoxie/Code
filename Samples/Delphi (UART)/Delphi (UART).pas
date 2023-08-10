unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Ariphm, ExtCtrls,DateUtils;

const
  wmCOMPORT=wm_User+11; {сообщение от порта}

type
  TForm1 = class(TForm)
    Button1: TButton;
    Label7: TLabel;
    ComboBox1: TComboBox;
    Label8: TLabel;
    Memo1: TMemo;
    Timer1: TTimer;
    procedure Button1Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure ComboBox1Select(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private
    { Private declarations }
  procedure ReceiveCOM(var MSG:TMessage); message wmCOMPORT;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  hCOM:hFile=0;
  pDCB:TDCB;
  comtime:TCOMMTIMEOUTS;
  xb:byte;
  xn:dword;
  ab: array[1..1024] of byte;
  st,stcom:string;
  ttime,told:TDateTime;

  ThreadID:dword;
  COMThread : THandle;
  StatCOM : TComStat;
  FlagCOM:boolean=False;
  StrOvr : TOverlapped;
  bb:boolean=True;

implementation

{$R *.dfm}

procedure ReadComPort; {это поток и есть}
var TMask:DWord;
begin
  while bb do   {бесконечный цикл}
  begin
    if not WaitCommEvent(hCOM,TMask,@StrOvr) then
    {запускаем прием}
    if GetLastError = ERROR_IO_PENDING
    then WaitForSingleObject(StrOvr.hEvent, INFINITE);
                      {ожидаем приема до бесконечности}
    ClearCommError(hCOM,xn,@StatCOM);
                         {получаем состо€ние —ќћ в StatCOM}
    xn:=StatCOM.cbInQue;
        {в StatCOM.cbInQue - реальное количество байт в буфере}
    if xn > 0 then
    if ReadFile(hCOM,ab,xn,xn,@StrOvr) then {читаем байты в ab}
      SendMessage(Form1.Handle,wmCOMPORT,1,0); {посылаем сообщение}
  end;
end;

procedure TForm1.ReceiveCOM(var MSG:TMessage);
{чтение очередного байта по сообщению wmCOMPORT}
var i:integer;
begin
  ttime:=Time; {фиксируем врем€ прихода байта}
  if MilliSecondsBetween(told,ttime)>500 then {если больше полсекунды очищаем строку}
  st:='';
   for i:=1 to xn do st:=st+hexb(ab[i])+' ';
   if Form1.Memo1.Lines.Count>65000 then Form1.Memo1.Lines.Clear;
   {больше 64  строк нельз€}
   Form1.Memo1.Lines.Add(st); {выводим в Memo}
   told:=ttime; {дл€ сравнени€ в следующий раз}
   if FlagCOM then {если это была посылка}
   begin
      FlagCOM:=False;
      st:='';  {очищаем строку}
      Timer1.Enabled:=False; {выключаем таймер}
   end;
end;

procedure IniCOM;
var i:integer;
begin
{инициализаци€ COM - номер с строке stcom}
  hCOM:=CreateFile(Pchar(stcom), GENERIC_READ+GENERIC_WRITE,0,nil,OPEN_EXISTING,
  FILE_FLAG_OVERLAPPED,0);
  if (hCom = INVALID_HANDLE_VALUE) then
  begin
    st:=stcom+' не найден';
    Application.MessageBox(Pchar(st),'Error',MB_OK);
    exit;
  end;
  if GetCommState(hCOM,pDCB)
  then  st:=stcom+': baud=9600 parity=N data=8 stop=1';
  if BuildCommDCB(Pchar(st),pDCB) then SetCommState(hCOM,pDCB)
  else
  begin
    st:=stcom+' зан€т или заданы неверные параметры';
    Application.MessageBox(Pchar(st),'Error',MB_OK);
    exit;
  end;
  GetCommTimeouts(hCom,comtime); {устанавливаем задержки:}
  comtime.WriteTotalTimeoutMultiplier:=1;
  comtime.WriteTotalTimeoutConstant:=10;
  comtime.ReadIntervalTimeout:=10;
  comtime.ReadTotalTimeoutMultiplier:=1;
  comtime.ReadTotalTimeoutConstant:=2000; {ждем чтени€ 2 сек}
  SetCommTimeouts(hCom,comtime);

  StrOvr.hEvent:=CreateEvent(nil,True,False,nil);

  ab[1]:=ord('A');  {будем посылать инициализацию модема}
  ab[2]:=ord('T');
  ab[3]:=13;{CR}
  ab[4]:=10;{LF}
  WriteFile(hCOM,ab,4,xn,@StrOvr);
  if ReadFile(hCOM,ab,10,xn,@StrOvr) then {ответ модема 10 знаков}
  begin
     st:='';
     for i:=1 to 10 do st:=st+chr(ab[i]);
     if pos('OK',st)<>0 then
     begin
       st:=stcom+' зан€т модемом';
       Application.MessageBox(Pchar(st),'Error',MB_OK);
       CloseHandle(hCOM);
       hCOM:=0;
       Form1.Label7.Caption:='COM?';
       exit;
     end;
  end;
  Form1.Label7.Caption:=stcom+' 9600';
  SetCommMask(hCOM,EV_RXCHAR); {отслеживаем событие в буфере}
  COMThread := CreateThread(nil,0,@ReadComPort,nil,0,ThreadID);
                                            {создаем поток}
  told:=Time; {фиксируем врем€ запуска}
  st:=''; {очищаем строку дл€ вывода}
end;

procedure TForm1.Button1Click(Sender: TObject);
begin    {запрос}
  if (hCOM=0) or (hCOM=INVALID_HANDLE_VALUE) then exit;
  {если порт еще не инициализирован - выход}
  PurgeComm(hCOM,PURGE_RXCLEAR); {очищаем буфер на вс€кий случай}
  xb:=$A2;
  WriteFile(hCOM,xb,1,xn,@StrOvr);
  st:='ѕослано: '+hexb(xb);
  Form1.Memo1.Lines.Add(st); {записали в Memo}
  st:=''; {очистили строку}
  told:=Time; {зафиксировали момент посылки}
  FlagCOM:=True; {обозначаем посылку}
  Timer1.Enabled:=True;  {запускаем таймер}
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
{инициализаци€ COM1 при запуске}
  stcom:='COM1';
  IniCOM;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin {уничтожаем COM}
    TerminateThread(COMThread,0); {уничтожаем поток}
    CloseHandle(hCOM); {закрываем —ќћ}
end;

procedure TForm1.ComboBox1Select(Sender: TObject);
begin
  TerminateThread(COMThread,0); {уничтожаем поток}
  CloseHandle(hCOM); {закрываем старый —ќћ}
  stcom:=ComboBox1.Text;  {устанавливаем порт COM1,2,3,4}
  IniCOM;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin   {таймер на ошибку}
  ttime:=Time;
  if SecondsBetween(told,ttime)>1 then {если через две секунды ничего нет}
  begin
   Timer1.Enabled:=False;
   FlagCOM:=False;
   Application.MessageBox('”стройство не обнаружено','Error',MB_OK);
  end;
end;

end.
