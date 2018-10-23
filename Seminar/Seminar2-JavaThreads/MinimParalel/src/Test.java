
public class Test {

	public static void main(String[] args) throws InterruptedException {

		//generare vector
		final int N = 300000000; 
		int[] valori = Vector.generareVector(N);
		
		//varianta secventiala
		long tStart = System.currentTimeMillis();
		int minim = Vector.minim(valori, 0, N);
		long tFinal = System.currentTimeMillis();
		
		System.out.println("Minim: "+minim);
		Vector.print(tStart, tFinal);
		
		//varianta cu thread-uri + resursa partajata
		int nThreaduri = 4;
		//minim global
		MinimGlobal minimGlobal = new MinimGlobal(valori[0]);
		//fire de executie
		Thread[] fireExecutie = new Thread[nThreaduri];
		for(int i=0;i<nThreaduri;i++) {
			int iStart = i*N/nThreaduri;
			int iFinal = (i+1)*N/nThreaduri;
			if(i == nThreaduri-1)
				iFinal = N;
			fireExecutie[i] = new ThreadMinim(
					i, iStart, iFinal, valori, minimGlobal);
		}
		
		tStart = System.currentTimeMillis();
		for(Thread thread : fireExecutie)
			thread.start();
		
		for(Thread thread : fireExecutie)
			thread.join();
		tFinal = System.currentTimeMillis();
		
		System.out.println("Nr threaduri: "+nThreaduri);
		System.out.println("Minim determinat paralel: "+minimGlobal.getMinim());
		Vector.print(tStart, tFinal);
		
		
		
		//varianta cu thread-uri + resursa locala

		//fire de executie
		Thread[] fireExecutieLocale = new Thread[nThreaduri];
				for(int i=0;i<nThreaduri;i++) {
					int iStart = i*N/nThreaduri;
					int iFinal = (i+1)*N/nThreaduri;
					if(i == nThreaduri-1)
						iFinal = N;
					fireExecutieLocale[i] = new ThreadMinimLocal(
							i, iStart, iFinal, valori);
				}
				
				tStart = System.currentTimeMillis();
				for(Thread thread : fireExecutieLocale)
					thread.start();
				
				for(Thread thread : fireExecutieLocale)
					thread.join();
				tFinal = System.currentTimeMillis();
				
				int minimLocal = 
						((ThreadMinimLocal)fireExecutieLocale[0]).getMinimLocal();
				for(int i=1;i<nThreaduri;i++)
					if(minimLocal > ((ThreadMinimLocal)fireExecutieLocale[i]).getMinimLocal())
						minimLocal = ((ThreadMinimLocal)fireExecutieLocale[i]).getMinimLocal();
				
				System.out.println("Nr threaduri locale: "+nThreaduri);
				System.out.println("Minim determinat paralel: "+minimLocal);
				Vector.print(tStart, tFinal);
	}

}
