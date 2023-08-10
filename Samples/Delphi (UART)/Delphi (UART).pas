unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Ariphm, ExtCtrls,DateUtils;

const
  wmCOMPORT=wm_User+11; {��������� �� �����}

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

procedure ReadComPort; {��� ����� � ����}
var TMask:DWord;
begin
  while bb do   {����������� ����}
  begin
    if not WaitCommEvent(hCOM,TMask,@StrOvr) then
    {��������� �����}
    if GetLastError = ERROR_IO_PENDING
    then WaitForSingleObject(StrOvr.hEvent, INFINITE);
                      {������� ������ �� �������������}
    ClearCommError(hCOM,xn,@StatCOM);
                         {�������� ��������� ��� � StatCOM}
    xn:=StatCOM.cbInQue;
        {� StatCOM.cbInQue - �������� ���������� ���� � ������}
    if xn > 0 then
    if ReadFile(hCOM,ab,xn,xn,@StrOvr) then {������ ����� � ab}
      SendMessage(Form1.Handle,wmCOMPORT,1,0); {�������� ���������}
  end;
end;

procedure TForm1.ReceiveCOM(var MSG:TMessage);
{������ ���������� ����� �� ��������� wmCOMPORT}
var i:integer;
begin
  ttime:=Time; {��������� ����� ������� �����}
  if MilliSecondsBetween(told,ttime)>500 then {���� ������ ���������� ������� ������}
  st:='';
   for i:=1 to xn do st:=st+hexb(ab[i])+' ';
   if Form1.Memo1.Lines.Count>65000 then Form1.Memo1.Lines.Clear;
   {������ 64� ����� ������}
   Form1.Memo1.Lines.Add(st); {������� � Memo}
   told:=ttime; {��� ��������� � ��������� ���}
   if FlagCOM then {���� ��� ���� �������}
   begin
      FlagCOM:=False;
      st:='';  {������� ������}
      Timer1.Enabled:=False; {��������� ������}
   end;
end;

procedure IniCOM;
var i:integer;
begin
{������������� COM - ����� � ������ stcom}
  hCOM:=CreateFile(Pchar(stcom), GENERIC_READ+GENERIC_WRITE,0,nil,OPEN_EXISTING,
  FILE_FLAG_OVERLAPPED,0);
  if (hCom = INVALID_HANDLE_VALUE) then
  begin
    st:=stcom+' �� ������';
    Application.MessageBox(Pchar(st),'Error',MB_OK);
    exit;
  end;
  if GetCommState(hCOM,pDCB)
  then  st:=stcom+': baud=9600 parity=N data=8 stop=1';
  if BuildCommDCB(Pchar(st),pDCB) then SetCommState(hCOM,pDCB)
  else
  begin
    st:=stcom+' ����� ��� ������ �������� ���������';
    Application.MessageBox(Pchar(st),'Error',MB_OK);
    exit;
  end;
  GetCommTimeouts(hCom,comtime); {������������� ��������:}
  comtime.WriteTotalTimeoutMultiplier:=1;
  comtime.WriteTotalTimeoutConstant:=10;
  comtime.ReadIntervalTimeout:=10;
  comtime.ReadTotalTimeoutMultiplier:=1;
  comtime.ReadTotalTimeoutConstant:=2000; {���� ������ 2 ���}
  SetCommTimeouts(hCom,comtime);

  StrOvr.hEvent:=CreateEvent(nil,True,False,nil);

  ab[1]:=ord('A');  {����� �������� ������������� ������}
  ab[2]:=ord('T');
  ab[3]:=13;{CR}
  ab[4]:=10;{LF}
  WriteFile(hCOM,ab,4,xn,@StrOvr);
  if ReadFile(hCOM,ab,10,xn,@StrOvr) then {����� ������ 10 ������}
  begin
     st:='';
     for i:=1 to 10 do st:=st+chr(ab[i]);
     if pos('OK',st)<>0 then
     begin
       st:=stcom+' ����� �������';
       Application.MessageBox(Pchar(st),'Error',MB_OK);
       CloseHandle(hCOM);
       hCOM:=0;
       Form1.Label7.Caption:='COM?';
       exit;
     end;
  end;
  Form1.Label7.Caption:=stcom+' 9600';
  SetCommMask(hCOM,EV_RXCHAR); {����������� ������� � ������}
  COMThread := CreateThread(nil,0,@ReadComPort,nil,0,ThreadID);
                                            {������� �����}
  told:=Time; {��������� ����� �������}
  st:=''; {������� ������ ��� ������}
end;

procedure TForm1.Button1Click(Sender: TObject);
begin    {������}
  if (hCOM=0) or (hCOM=INVALID_HANDLE_VALUE) then exit;
  {���� ���� ��� �� ��������������� - �����}
  PurgeComm(hCOM,PURGE_RXCLEAR); {������� ����� �� ������ ������}
  xb:=$A2;
  WriteFile(hCOM,xb,1,xn,@StrOvr);
  st:='�������: '+hexb(xb);
  Form1.Memo1.Lines.Add(st); {�������� � Memo}
  st:=''; {�������� ������}
  told:=Time; {������������� ������ �������}
  FlagCOM:=True; {���������� �������}
  Timer1.Enabled:=True;  {��������� ������}
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
{������������� COM1 ��� �������}
  stcom:='COM1';
  IniCOM;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin {���������� COM}
    TerminateThread(COMThread,0); {���������� �����}
    CloseHandle(hCOM); {��������� ���}
end;

procedure TForm1.ComboBox1Select(Sender: TObject);
begin
  TerminateThread(COMThread,0); {���������� �����}
  CloseHandle(hCOM); {��������� ������ ���}
  stcom:=ComboBox1.Text;  {������������� ���� COM1,2,3,4}
  IniCOM;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin   {������ �� ������}
  ttime:=Time;
  if SecondsBetween(told,ttime)>1 then {���� ����� ��� ������� ������ ���}
  begin
   Timer1.Enabled:=False;
   FlagCOM:=False;
   Application.MessageBox('���������� �� ����������','Error',MB_OK);
  end;
end;

end.
