
public class ThreadMinimLocal extends Thread{
	private int ID;
	private int iStart;
	private int iFinal;
	private int[] valori;
	private int minimLocal;
	
	public ThreadMinimLocal(int iD, int iStart, int iFinal, int[] valori) {
		super();
		ID = iD;
		this.iStart = iStart;
		this.iFinal = iFinal;
		this.valori = valori;
		minimLocal = valori[iStart];
	}
	
	public void run() {
		for(int i=iStart;i<iFinal;i++)
			if(minimLocal > valori[i])
				minimLocal = valori[i];
	}

	public int getMinimLocal() {
		return minimLocal;
	}
	
	
}
