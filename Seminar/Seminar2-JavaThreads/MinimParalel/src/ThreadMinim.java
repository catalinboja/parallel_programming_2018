
public class ThreadMinim extends Thread{
	private int ID;
	private int iStart;
	private int iFinal;
	private int[] valori;
	private MinimGlobal minim;
	
	public ThreadMinim(int iD, int iStart, int iFinal, int[] valori, MinimGlobal minim) {
		super();
		ID = iD;
		this.iStart = iStart;
		this.iFinal = iFinal;
		this.valori = valori;
		this.minim = minim;
	}
	
	public void run() {
		for(int i=iStart;i<iFinal;i++)
			minim.verificaSiModifica(valori[i]);
	}
}
