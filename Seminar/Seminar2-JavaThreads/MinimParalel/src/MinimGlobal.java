
public class MinimGlobal {
	private int minim;
	
	public void verificaSiModifica(int valoare) {
		if(minim > valoare)
			minim = valoare;
	}

	public MinimGlobal(int minim) {
		super();
		this.minim = minim;
	}

	public int getMinim() {
		return minim;
	}
	
	
}
