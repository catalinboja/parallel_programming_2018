import java.util.Random;

public class Vector {
	
	static int[] generareVector(int n) {
		int[] valori = new int[n];
		Random rand = new Random();
		
		for(int i=0;i<n;i++)
			valori[i] = rand.nextInt(n);
		
		return valori;
	}
	
	//minim secvential
	static int minim(int[] valori, int iStart, int iFinal) {
		int min = valori[0];
		for(int i=iStart;i<iFinal; i++)
			if(min > valori[i])
				min = valori[i];
		return min;
	}
	
	static void print(long tStart, long tFinal) {
		String mesaj = 
				String.format("Durata: %d ms", tFinal-tStart);
		System.out.println(mesaj);
	}
	
}
