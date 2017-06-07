


setwd("M:/Documents/Programmierung/git/LAVESI")

filespos=list.files("./output", pattern="dataentwicklung_positionsanalyse_1_22")
par(mfrow=c(1,length(filespos)))
for(fi in filespos)
{
	dpos=read.csv2(paste0("./output/",fi), dec=".")
	str(dpos)

	with(dpos, plot(Y~X, pch=16, main=fi, col=ifelse(Y>50000,"orange","forestgreen")))
}

# focus auf grenze
	with(dpos[dpos$Y>50000-100,], plot(Y~X, pch=16, ylim=c(50000,51000)-100, main=fi, col=ifelse(Y>50000,"orange","forestgreen")))




dcur=read.csv2("./output/dataentwicklung_currencies_1_22.csv", dec=".")
str(dcur)
