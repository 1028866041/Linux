import java.io.*;
import java.net.*;
import java.util.*;
import java.util.logging.*;

public class HttpRedirector{

	private static final Logger logger= Logger.getLogger("HttpRedirector");
	private final int port;
	private final String newSite;

	public HttpRedirector(String newSite, int  port){
		this.port= port;
		this.newSite= newSite;	
	}

	public void start(){
		try(ServerSocket server= new ServerSocket(port)){

			while(true){
				try{
					Socket s= server.accept();
					Thread t= new RedirectThread(s);
					t.start();
				}catch(IOException e){
					logger.warning("connect exception");
				}catch(RuntimeException e){
					logger.log(Level.SEVERE, "Unexpected error");
				}				
			}
		}catch(BindException e){
			logger.log(Level.SEVERE, "Unexpected bind error", e);
		}catch(IOException e){
			logger.log(Level.SEVERE, "Server start error", e);
		}
		/*logger.info("Redirect connection on port"+server.getLocalPort()+"to"+newSite);*/
	}
	
	public class RedirectThread extends Thread{
		private final Socket connect;

		RedirectThread(Socket s){
			this.connect= s;
		}

		public void run(){
			try{
				Writer out= new BufferedWriter(new OutputStreamWriter(connect.getOutputStream(),"US-ASCII"));
				Reader in= new InputStreamReader(new BufferedInputStream(connect.getInputStream()));

				StringBuilder request= new StringBuilder(80);
				while(true){
					int c= in.read();
					if(c=='\r'||c=='\n'||c==-1)
						break;
					request.append((char)c);
				}
				
				String get= request.toString();
				String[] pieces= get.split("\\w");
				String theFile= pieces[1];

				/*mime header*/
				out.write("HTTP/1.0 302 FOUND\r\n");
				Date now= new Date();
				out.write("Date:"+now+"\r\n");
				out.write("Server: Redirector 1.1\r\n");
				out.write("Location:"+newSite+theFile+"\r\n");
				out.write("Content-type: text/html\r\n\r\n");

				/*Where to move file*/
				out.write("<HTML><HEAD><TITLE>Move</TITLE>");
				out.write("<BODY><H1>Document moved</H1>\r\n");
				out.write("The document"+theFile+"has moved to\r\n<A HREF=\""+newSite+theFile+"\">"+
					newSite+theFile+"</A>.\r\n Update bookmarks<P>");
				out.write("</BODY></HTML>\r\n");				
				out.flush();	
				
				logger.log(Level.INFO, "Redirected"+ connect.getRemoteSocketAddress());
			}catch(IOException e){
				logger.log(Level.WARNING, "error connect to"+connect.getRemoteSocketAddress(), e);
			}finally{
				try{
					connect.close();
				}catch(IOException e){
					/*nothing*/
				}
			}
		}
	}

	public static void main(String args[]){
		int thePort;
		String theSite= null;

		try{
			theSite= args[0];
			if(theSite.endsWith("/"))
				theSite= theSite.substring(0, theSite.length()-1);
		}catch(RuntimeException e){
			System.exit(1);
		}

		try{
			thePort= Integer.parseInt(args[1]);
		}catch(RuntimeException e){
			thePort= 80;
		}

		HttpRedirector redirector= new HttpRedirector(theSite, thePort);
		redirector.start();
	}
}

