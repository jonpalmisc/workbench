run:	playground.py
	mkdir -p out
	python3 playground.py

playground.py: playground.ipynb
	jupyter nbconvert --to script $<

clean:
	rm -fr out
