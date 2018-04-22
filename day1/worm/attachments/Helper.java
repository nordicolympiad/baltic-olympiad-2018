import java.io.*;

class Grader {
	public int N, M, K, Q;

	public Grader() {
		try {
			String[] ar = new BufferedReader(new InputStreamReader(System.in)).readLine().split(" ");
			N = Integer.parseInt(ar[0]);
			M = Integer.parseInt(ar[1]);
			K = Integer.parseInt(ar[2]);
			Q = Integer.parseInt(ar[3]);
			is = new FileInputStream(FileDescriptor.in);
			os = new FileOutputStream(FileDescriptor.out);
			buf = new byte[100];
		} catch(Exception e) { throw new RuntimeException(e); }
	}

	public int query(int x, int y, int z) {
		try {
			os.write(("? " + x + " " + y + " " + z + "\n").getBytes());
			int ret = 0;
			for (;;) {
				int count = is.read(buf);
				if (count == 0 || buf[0] == 45) System.exit(0);
				for (int i = 0; i < count; i++) {
					if (buf[i] == 10) return ret;
					ret *= 10;
					ret += (int)(buf[i] - 48);
				}
			}
		} catch(Exception e) { throw new RuntimeException(e); }
	}

	public void guess(int x, int y, int z) {
		try {
			os.write(("! " + x + " " + y + " " + z + "\n").getBytes());
			System.exit(0);
		} catch(Exception e) { throw new RuntimeException(e); }
	}

	private FileInputStream is;
	private FileOutputStream os;
	private byte[] buf;
}


class Main {
	public static void main(String[] args) {
		Grader g = new Grader();

		// TODO do something smart

		g.query(1, 1, 1);
		g.query(g.N, g.M, g.K);
		g.guess(1, 1, 1);
	}
}
