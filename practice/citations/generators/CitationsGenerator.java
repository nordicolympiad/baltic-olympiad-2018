import java.util.*;
import java.io.*;

public class CitationsGenerator {
	
	static String mainDir = "../data/secret";
	static String taskname = "citations";
	static String group1Dir = "g1/";
	static String group2Dir = "g2/";
	static String group3Dir = "g3/";
	static String group4Dir = "g4/";
	
	static Random rn;
	
	public static void main(String[] args) {
		rn = new Random();
		genGroup1();
		genGroup2();
		genGroup3();
		genGroup4();
	}
	
	public static void genGroup1() {
		String prefix = mainDir + group1Dir + taskname;
		
		genRandomTreeInstance(prefix + ".g1.01.in", 10, 1000, 2);
		genRandomTreeInstance(prefix + ".g1.02.in", 10, 1000, 3);
		genRandomTreeInstance(prefix + ".g1.03.in", 10, 1000 ,4);
		genRandomTreeInstance(prefix + ".g1.04.in", 10, 1000 ,5);
		genRandomTreeInstance(prefix + ".g1.05.in", 10, 1000 ,6);
		genRandomTreeInstance(prefix + ".g1.06.in", 10, 1000 ,7);
		genRandomTreeInstance(prefix + ".g1.07.in", 10, 1000 ,8);
		genRandomTreeInstance(prefix + ".g1.08.in", 10, 1000 ,9);
		genRandomTreeInstance(prefix + ".g1.09.in", 10, 1000 ,10);
		genRandomTreeInstance(prefix + ".g1.10.in", 10, 1000 ,10);
		
	}

	
	public static void genGroup2() {
		String prefix = mainDir + group2Dir + taskname;
		
		genRandomTreeInstance(prefix + ".g2.01.in", 50, 10, 2);
		genRandomTreeInstance(prefix + ".g2.02.in", 50, 10, 2);
		genRandomTreeInstance(prefix + ".g2.03.in", 50, 10 ,3);
		genRandomTreeInstance(prefix + ".g2.04.in", 50, 10 ,3);
		genRandomTreeInstance(prefix + ".g2.05.in", 50, 10 ,4);
		genRandomTreeInstance(prefix + ".g2.06.in", 50, 10 ,4);
		genRandomTreeInstance(prefix + ".g2.07.in", 50, 10 ,5);
		genRandomTreeInstance(prefix + ".g2.08.in", 50, 10 ,5);
		genRandomTreeInstance(prefix + ".g2.09.in", 50, 10 ,5);
		genRandomTreeInstance(prefix + ".g2.10.in", 50, 10 ,5);

	}	
	
	public static void genGroup3() {
		String prefix = mainDir + group3Dir + taskname;
		
		genRandomTreeInstance(prefix + ".g3.01.in", 20000, 1000, 3);
		genRandomTreeInstance(prefix + ".g3.02.in", 30000, 1000, 3);
		genRandomTreeInstance(prefix + ".g3.03.in", 30000, 1000 ,3);
		genRandomTreeInstance(prefix + ".g3.04.in", 50000, 1000 ,3);
		genRandomTreeInstance(prefix + ".g3.05.in", 50000, 1000 ,4);
		genRandomTreeInstance(prefix + ".g3.06.in", 100000, 1000 ,4);
		genRandomTreeInstance(prefix + ".g3.07.in", 100000, 1000 ,5);
		
		genCaterpillarInstance(prefix + ".g3.08.in", 20000, 10000, 1000, 5);
		genCaterpillarInstance(prefix + ".g3.09.in", 50000, 20000, 1000, 5);
		genCaterpillarInstance(prefix + ".g3.10.in", 45000, 35000, 1000, 5);
	}
	
	
	public static void genGroup4() {
		String prefix = mainDir + group4Dir + taskname;
		
		genShallowTreeInstance(prefix + ".g4.01.in", 1, 90000, 1000);
		genShallowTreeInstance(prefix + ".g4.02.in", 10, 90000, 1000);
	    genShallowTreeInstance(prefix + ".g4.03.in", 500, 90000, 1000);		
	    genShallowTreeInstance(prefix + ".g4.04.in", 1000, 90000, 1000);
	
		genRandomTreeInstance(prefix + ".g4.05.in", 50000, 1000 ,1000);
		genRandomTreeInstance(prefix + ".g4.06.in", 100000, 1000 ,1000);
		genRandomTreeInstance(prefix + ".g4.07.in", 100000, 1000 ,1000);
		
		genCaterpillarInstance(prefix + ".g4.08.in", 20000, 10000, 1000, 5);
		genCaterpillarInstance(prefix + ".g4.09.in", 50000, 20000, 1000, 5);
		genCaterpillarInstance(prefix + ".g4.10.in", 45000, 35000, 1000, 5);
	}
	
	// --------------------------------------------------------------------------
	
	
	public static void genRandomTreeInstance(String fileName, int n, int maxTime, int maxDegree) {
		RandomTreeGenerator gen = new RandomTreeGenerator();
		
		try{
		    PrintWriter writer = new PrintWriter(fileName, "UTF-8");
		    writer.println(n);
		    
		    ArrayList<ArrayList<Integer> > T = gen.randomBoundedDegreeRootedTreeNoParent(n, maxDegree);
		    
		    printTree(T,maxTime,writer);
			
		    writer.close();
		} catch (IOException e) {
		   // do something
		}
	}
	
	public static void genShallowTreeInstance(String fileName, int inner, int leaves, int maxTime) {
		RandomTreeGenerator gen = new RandomTreeGenerator();
		int n = inner + leaves; 		
		
		try{
		    PrintWriter writer = new PrintWriter(fileName, "UTF-8");
		    writer.println(n);
		    
		    
		    
		    ArrayList<ArrayList<Integer> > T = gen.randomRootedShallowTreeNoParent(inner, leaves);
		    
		    printTree(T,maxTime,writer);
		    
		    writer.close();
		} catch (IOException e) {
		   // do something
		}		
	}
	
	public static void genCaterpillarInstance(String fileName, int inner, int leaves, int maxTime, int maxDegree) {
		RandomTreeGenerator gen = new RandomTreeGenerator();
		int n = inner + leaves; 
		
		try{
		    PrintWriter writer = new PrintWriter(fileName, "UTF-8");
		    writer.println(n);
		
		    ArrayList<ArrayList<Integer> > T = gen.randomRootedCaterpillarNoParent(inner, leaves);
		    
		    printTree(T,maxTime,writer);
		    
		    writer.close();
		} catch (IOException e) {
		   // do something
		}
	}
	
	public static void printTree (ArrayList<ArrayList<Integer> > T, int maxTime, PrintWriter writer) throws IOException {
		for(int i = 0; i < T.size(); ++i) {
			writer.print((1 + rn.nextInt(maxTime-1)) + " " + T.get(i).size());
			for(int j : T.get(i)) writer.print(" " + (j+1));
			writer.println();
		}		
	}
	
	
	
}
