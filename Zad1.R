rm(list=ls())

x <- c(.62, .54, .21, .48, .71, .58, .32, .48, .55, .38)
x <- sort(x)

lambda <- 1.36

n <- length(x)

f <- seq(1, by=1, length=10)

#Empirijska funkcija raspodele
Fn <- f/n

#Data funkcija raspodele
Fx <- punif(x, 0, 1)

#Realizovana vrednost statistike
rvs <- sqrt(n) * max(abs(Fn - Fx))

sprintf("y0 = %.6f < 1.36 => hipoteza H0 se ne odbacuje!", rvs, lambda)

#2. zadatak

rm(list=ls())

# H0 : obelezja X i Y su nezavisna

fij <- matrix(c(11, 21, 17, 8, 29, 14), 2, 3)

# Matrica kontingencije

f <- matrix(nrow = nrow(fij) + 1, ncol = ncol(fij) + 1 )

for (i in 1:nrow(f)) {
  for (j in 1:ncol(f)) {
    if (i < nrow(f) & j < ncol(f)) {
      f[i,j] = fij[i,j] 
    } else if (i < nrow(f) & j == ncol(f)) { 
      f[i,j] = sum(fij[i,])
    } else if (i == nrow(f) & j < ncol(f)) { 
      f[i,j] = sum(fij[,j]) 
    } else if (i == nrow(f) & j == ncol(f)) { 
      f[i,j] = sum(f[1:i-1,j]) 
    }
  } 
}

# Obim uzorka
n <- sum(fij)

# Teorijske frekvencije
ft <- matrix(NA, nrow(fij), ncol(fij))

for (i in 1:nrow(ft)) {
  for (j in 1:ncol(ft)) {
    ft[i,j] <- f[i, ncol(f)] * f[nrow(f), j] / n
  }
}

# Realizovana vrednost statistike
yo <- sum((ft - fij)^2 / ft)

# Najveca dozvoljena vrednost statistike
y <- qchisq(1 - .05, (nrow(fij)-1) * (ncol(fij) - 1))

sprintf("y0 = %.6f > y = %.6f => hipoteza H0 se odbacuje!", yo, y)

#3. zadatak

rm(list=ls())

x <- c(.33, .19, .24, .44, .02,
       .89,.06, .05, .13, .08,
       .16, .35, .86, .05, .11,
       .07, .04, .09, .52, .38)

mi <- c(-Inf, .1, .25, Inf)
fi <- table(cut(x, mi, right = TRUE, include.lowest = TRUE))
print(fi)

n <- sum(fi)
xn <- mean(x)

lambda <- 1 / xn
k <- length(fi)
s <- 1

# Funkcija raspodele
Fx <- function(x) {
  if (x < 0) {
    fx = 0
  }
  else {
    fx = 1 - exp(-lambda * x)
  }
  
  return (fx)
}

# Teorijske verovatnoce
p <- numeric(k)
for (i in 1:k) {
  p[i] <- Fx(mi[i+1]) - Fx(mi[i])
}

#Realizovana vrednost statistike
y0 <- sum((fi - n*p)^2 /n / p)

#Najveca dozvoljena vrednost statistike
z <- qchisq(1 - .05, k - 1 - s)

sprintf("y0 = %.6f < y = %.6f => hipoteza H0 se ne odbacuje!", y0, z)

#9. VEZBE, ZADATAK 6

# testiranje hipoteze o jednakosti srednjih vrednosti dva obelezja
podaci< − read.csv("prijemni.csv")
prijemniM< − podaci$prijemni[podaci$pol=="m"]
prijemniZ< − podaci$prijemni[podaci$pol=="z"]
n1< − length(prijemniM)
n2< − length(prijemniZ)
xn1< − mean(prijemniM)
xn2< − mean(prijemniZ)
s1< − sd(prijemniM)^2∗(n1 − 1)/n1
s2< − sd(prijemniZ)^2∗(n2 − 1)/n2
t< − qt((1+.95)/2,n1+n2 − 2)
t0< − (xn1 − xn2)/sqrt((n1∗s1+n2∗s2)/(n1+n2 − 2))/sqrt(1/n1+1/n2)

#Nulta hipoteza H 0 ( m 1 = m 2 )
#t 0 = − 1.739699 ∈ I = (− 2.026192, 2.026192 ) ⇒ hipoteza H 0 se prihvata

#9. VEZBE, ZADATAK 8

Testirati hipotezu da je uspeh u srednjoj školi raspored̄en po normalnoj raspodeli ako
su grupe za χ 2 -test: 10-25, 25-30, 30-35, 35-40 (α = 0.05).

podaci< − read.csv("prijemni.csv")
uspehSkola< − podaci$skola
mi< − c(10,25,30,35,40)
table(cut(uspehSkola,mi,include.lowest=TRUE,right=TRUE))
fi < − c(10,11,11,7)
n< − length(uspehSkola)
k< − length(fi) #broj intervala
s< − 2 #broj parametara koje treba oceniti
xn< − mean(uspehSkola)
sn< − sd(uspehSkola)
#funkcija raspodele
Fx< − function(x){
return(pnorm((x − xn)/sn)
}
# teorijske verovatnoce
p< − numeric(length(fi))
p[1]< − Fx(mi[2]) − Fx( − Inf)
p[2]< − Fx(mi[3]) − Fx(mi[2])
p[3]< − Fx(mi[4]) − Fx(mi[3])
p[4]< − Fx(Inf) − Fx(mi[4])

#realizovana vrednost statistike
y0< − sum((fi − n∗p)^2/n/p)
#najveca dozvoljena vrednost statistike
y< − qchisq(1 − .05,k − 1 − s)

y 0 = 1.296183 < y = 3.841459 ⇒ hipoteza H 0 se prihvata

#primer prvog iz fajla sa tabelom

Testom Kolmogorov-Smirnov testirati hipotezu o saglasnosti datog uzorka sa norma-
lnom raspodelom N ( 10, 4 ) (α = 0.05). Sa tabelom.

fi < − c(3,7,10,15,5)
mi< − c(0,5,15,20,30,40)
m0< − mi[1:length(mi) − 1]
m1< − mi[2:length(mi)]
xi< − (m0+m1)/2
f< − cumsum(fi)
n< − sum(fi)
Fn< − f/n #empirijska funkcija raspodele
Fx< − pnorm(xi,10,4) #data funkcija raspodele
#realizovana vrednost statistike
sqrt(n)∗max(abs(Fn − Fx))

2.970034 > 1.36 = λ 0.95 ⇒ hipoteza H 0 se odbacuje

#10. VEZBE, ZADATAK 5

Testirati hipotezu o jednakosti srednje vrednosti uspeha iz srednje škole po grupama
(ANOVA).

>podaci< − read.csv("prijemni.csv")
>anova(lm(skola~grupa,data=podaci))

Analysis of Variance Table

Response: skola
		  Df		Sum Sq  	   Mean Sq 		  F value 		Pr(>F)  
grupa 	  2     	26.6		   13.300		  0.4512		0.6404
Residuals 36		1061.2		   29.376		  

#isto dobijamo i sa komandom
summary(aov(lm(skola~grupa,data=podaci)))

H 0 ( m 1 = m 2 = m 3 ) protiv H 1 (∃ i, j, m i 6 = m j )
D f 1 SSTR MSTR F α ∗
D f 2
SSE
MSE
D f 1 , D f 2 − broj stepeni slobode
F − realizovana vrednost Fišerove test statistike sa D f 1 , D f 2 stepeni slobode
α ∗ − p-vrednost
Tabela ANOVE:
α ∗ = 0.6404 > α = 0.05 ⇒ H 0 se prihvata

#10. VEZBE, ZADATAK 4

podaci< − read.csv("prijemni.csv")
boxplot(skola~grupa,data=podaci)

#10. VEZBE, ZADATAK 6

Naći koeficijent korelacije uspeha iz srednje škole u zavisnosti od uspeha na prijem-
nom. Prognozirati kom uspehu u srednjoj školi odgovara 35 bodova osvojenih na
prijemnom.

podaci< − read.csv("prijemni.csv")
lm(skola~prijemni,data=podaci)


Call:
	lm(formula = skola ~ prijemni, data = podaci)
Coefficients :
(Intercept)
	29.784053     prijemni
				 − 0.008009

Jednačina linearne regresije
skola = − 0.008009 ∗ prijemni + 29.784053

plot(podaci$prijemni,podaci$skola)
abline(lm(skola~prijemni,data=podaci))

# koeficijent korelacije
cor(podaci$skola,podaci$prijemni)
#predikcija
lm(skola~prijemni,data=podaci)$coefficients%∗%c(1,35)

Koeficijent korelacije r = − 0.01157393
| r | < 0.3 ⇒ uspeh iz srednje škole i uspeh na prijemnom nisu u korelacionoj vezi


Broju od 35 poena osvojenih na prijemnom odgovara 29.50375 poena iz srednje škole.				 


#11.VEZBE, K-S TEST 

Kolmogorov-Smirnov testom testirati hipotezu o saglasnosti prvog uzorka sa norma-
lnom raspodelom N ( 10, 0.81 ) .

ks.test (uzorak1,’pnorm’,10,0.81)

One − sample Kolmogorov − Smirnov test
data: uzorak1
D = 0.17074, p − value = 0.3097
alternative hypothesis: two − sided

α ∗ = 0.3097 > α = 0.05 ⇒ hipoteza H 0 se prihvata

Napomena:
Vrednost D = 0.17074 je realizovana vrednost statistike D n = sup | F n ∗ ( x ) − F ( x )| .

Kako je sqrt(n1)*D = sqrt(30) · 0.17074 = 0.9351815 < 1.36 zaključujemo da se hipoteza H 0 prihvata.

#11.VEZBE, STUDENTOV-T Test

Testirati hipotezu da je srednja vrednost prvog uzorka veća od srednje vrednosti dru-
gog

t. test (uzorak1,uzorak2,alternative=’greater’)


Welch Two Sample t − test
data: uzorak1 and uzorak2
t = 5.3151, df = 77.935, p − value = 4.929e − 07
alternative hypothesis: true difference in means is greater than 0
95 percent confidence interval :
0.7067711 	Inf
sample estimates:
mean of x mean of y
10.10867 9.07960

H0 ( m 1 = m 2 ) protiv H 1 ( m 1 > m 2 )
α ∗ = 4.929e − 07 < α = 0.05 ⇒ hipoteza H 0 se odbacuje, tj. prihvata se H 1

#za studentov test t.test(). uzorak1, uzorak2. gleda se da li je p-value veci od alfa. ako jeste H0 se prihvata (da su isti)!

#11.VEZBE, STUDENTOV-T Test, pod pretpostavkom da su iste varijanse

Testirati hipotezu da je srednja vrednost prvog uzorka veća od srednje vrednosti dru-
gog, pod pretpostavkom da imaju jednake varijanse.

t. test (uzorak1,uzorak2,alternative=’greater’,var.equal=TRUE)

Two Sample t − test
data: uzorak1 and uzorak2
t = 4.6821, df = 78, p − value = 5.892e − 06
alternative hypothesis: true difference in means is greater than 0
95 percent confidence interval :
0.6631998	Inf
sample estimates:
mean of x mean of y
10.10867 9.07960

H0 ( m 1 = m 2 ) protiv H 1 ( m 1 > m 2 )
α ∗ = 5.892e − 06 < α = 0.05 ⇒ hipoteza H 0 se odbacuje, tj. prihvata se H 1

#11.VEZBE, TESTIRANJE STD. DEVIJACIJE DA JE JEDNAKA 1

Testirati hipotezu da je standardna devijacija prvog uzorka jednaka 1.

n< − length(uzorak1)
s< − sd(uzorak1)^2∗(n − 1)/n
y1< − qchisq((1+.95)/2,n − 1)
y2< − qchisq((1 − .95)/2,n − 1)
x1< − n∗s/y1
x2< − n∗s/y2

Nulta hipoteza H 0 ( σ = 1 )
σ 0 = 1 ∈/ I = ( 0.2552004, 0.7271325 ) ⇒ hipoteza H 0 se odbacuje #(x1, x2)


#11.VEZBE, testiranje da su std. varijacije JEDNAKE

Testirati hipotezu da su standardne devijacije kod oba uzorka jednake.

var.test(uzorak1,uzorak2)

F test to compare two variances
data: uzorak1 and uzorak2
F = 0.33427, num df = 29, denom df = 49, p − value = 0.002289
alternative hypothesis: true ratio of variances is not equal to 1
95 percent confidence interval :
0.1776679    0.6653103
sample estimates:
ratio of variances
	0.3342673

Nulta hipoteza H 0 ( σ 1 2 = σ 2 2 )
α ∗ = 0.002289 < α = 0.05 ⇒ hipoteza H 0 se odbacuje

#10.VEZBE, NEZAVISNOST OBELEZJA x i Y SA JEDNOM LINIJOM KODA

chisq.test(matrix(c(5,5,17,7,9,19),2,3))

Pearson’s Chi − squared test
data: fij
X − squared=7.7381, df=2, p − value=0.02088

H 0 : obeležja X i Y su nezavisna
α ∗ = 0.02088 < α = 0.05 ⇒ hipoteza H 0 se odbacuje
