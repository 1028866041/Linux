import java.io.*;
import java.util.*;
import java.net.*;

public class Sock{

	public static void main(String args[]){

		try{
			Socket sock= new Socket("localhost", 13);
			//sock.connect(new InetSocketAddress("localhost",13), 1000);
		
			InputStream is= sock.getInputStream();
			Scanner in= new Scanner(is);

			while(in.hasNextLine()){
				String line= in.nextLine();
				System.out.println(line);
			}

		}catch(InterruptedIOException e){
			e.printStackTrace();
		}catch(IOException e){
			e.printStackTrace();
		}
	}
}

