import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.w3c.dom.CharacterData;

public class Xml{

	public static void main(String[] args){	

		EventQueue.invokeAndWait(new Bundle()){
		public void run(){
			JFrame frame= new DOMTreeFrame();
			frame.setTitle("Xml");
			frame.setDfaultCloseOperation();
			frame.setVisible(true);
		}
	}
}

