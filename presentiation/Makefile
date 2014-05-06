# Latex Makefile

TEXFILE= presentation.tex
PDFTITLE= presentation

$(PDFTITLE).pdf:	
	pdflatex $(TEXFILE)


# alle Dateien loeschen, die von latex automatisch erzeugt werden
clean:
	rm -f *.dvi *.ps *.rtf *.aux *.bbl *.blg *.idx
	rm -f *.ilg *.ind *.log *.toc *.sec *.out *.synctex.gz
	rm -f *.gnuplot *.table *.vrb *.snm *.nav
