for i; do
	psftp 10.4.13.$i -l cs4404 -pw AITF -b putfile
done
