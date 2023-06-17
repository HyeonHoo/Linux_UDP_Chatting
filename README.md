# Launch Image
![UDP서버_클라이언트_통신](https://github.com/HyeonHoo/Linux_UDP_Chatting/assets/69030306/c4d40a70-c95b-488f-a35f-5860f6edca9c)

# 목적
UDP 서버와 클라이언트 데이터 송수신

# 설명
- UDP 소켓,sendto(),recvfrom(),write()read()
- UDP를 이용하여 서버와 클라이언트 간 통신을 구현하는 코드
- 사용자 입력을 위한 rfds[0], 소켓 입력을 위한 rfds[1]을 각각 0(표준 입력), nSockFd로 설정
- 소켓으로부터 데이터를 수신할 경우 recvfrom() 함수를 사용하여 데이터를 받은 후 화면에 출력
- 사용자로부터 입력을 받을 경우 sendto() 함수를 사용하여 데이터를 전송
