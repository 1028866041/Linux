import java.io.*;
import java.net.*;

public class Mcast{
	
	public static void main(String args[]){
		InetAddress ia= null;
		int port= 0;
		byte ttl= 1;
		
		try{	
			ia= InetAddress.getByName(args[0]);
			port= Integer.parseInt(args[1]);
			if(args.length> 2)
				ttl= (byte)Integer.parseInt(args[2]);
		}catch(NumberFormatException| IndexOutOfBoundsException|
			UnknownHostException e){
				e.printStackTrace();
				System.exit(1);
		}

		byte[] data= "Here's some multicast data\r\n".getBytes();
		DatagramPacket dp= new DatagramPacket(data, data.length, ia, port);

		try(MulticastSocket ms = new MulticastSocket()/*AutoCloseable*/){
			ms.setTimeToLive(ttl);
			ms.joinGroup(ia);
		    for(int i=0;i< 10;i++){
				ms.send(dp);
			}
			ms.leaveGroup(ia);
		}catch(SocketException e){
			e.printStackTrace();
			System.exit(1);		
		}catch(IOException e){
			e.printStackTrace();
		}
	}
}

