import java.util.*;

public class RandomPermutationGenerator {
	Random rn;
	RandomPermutationGenerator() {rn = new Random();}
	
	ArrayList<Integer> randomPermutaion(int n) {
		ArrayList<Integer> ans = new ArrayList<Integer> ();
		for(int i = 0; i < n; ++i) ans.add(i);
		for(int i = 0; i < n; ++i) {
			int j = rn.nextInt(n-i);
			int a = ans.get(i);
			int b = ans.get(i+j);
			ans.set(i, b);
			ans.set(i+j, a);
		}
		return ans;
	}
	
	public static void main(String[] args) {
		RandomPermutationGenerator gen = new RandomPermutationGenerator();
		System.out.println(gen.randomPermutaion(10));
	}
}
