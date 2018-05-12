import java.io.*;
import java.net.*;

//Sniffer
public class McastSniffer{

	public static void main(String args[]){
		InetAddress group = null;
		int port= 0;
		
		try{
			group = InetAddress.getByName(args[0]);
			port = Integer.parseInt(args[1]);
		}catch(ArrayIndexOutOfBoundsException| NumberFormatException|
			UnknownHostException e){
				e.printStackTrace();
				System.exit(1);
		}

		MulticastSocket ms = null;
		try{
			ms = new MulticastSocket(port);
			ms.joinGroup(group);
		
			byte[] buffer= new byte[8192];
			while(true)
			{
				DatagramPacket dp = new DatagramPacket(buffer, buffer.length);
				ms.receive(dp);
				String s= new String(dp.getData(), "8859_1");
				System.out.println(s);
			}
		}catch(IOException e){
			e.printStackTrace();	
		}finally{
			if(ms!= null){
				try{
					ms.leaveGroup(group);
					ms.close();
				}catch(IOException e){
					e.printStackTrace();
				}					
			}
		}
	}
}

