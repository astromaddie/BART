/*
 * test_dbread_pands.c - Test for the driver to read Partridge &
 *                       Schwenke for Transit. Part of Transit program.
 *
 * Copyright (C) 2003 Patricio Rojo (pato@astro.cornell.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#include "dbread_pands.c"


/*\fcnfh 
  Routine for testing
*/
int main(int argc,
	 char **argv)
{
  struct linedb *lines,*lp;
  PREC_NREC n;
  float wl1,wl2,szb,endb,tf1;
  int i,nbins,qb[NUM_ISOT],ti1;
  char *file="../oth/pands/h2ofast.bin";
  char *zfile="../oth/pands/h2opartfn.dat";
  int ans;
  PREC_ZREC **Z,*T,*isomass;
  int nT,nIso;
  char rc;

  wl1=1748.5;
  wl2=1828.5;
  nbins=20;

  printf("Welcome!\n");
  fprintf(stderr,"Lower wavelength (nm)[%g]?: ",wl1);
  if((tf1=readds(stdin,&rc,NULL,0))!=0||rc==-1)
    wl1=tf1;

  fprintf(stderr,"Upper wavelength (nm)[%g]?: ",wl2);
  if((tf1=readds(stdin,&rc,NULL,0))!=0||rc==-1)
    wl2=tf1;

  fprintf(stderr,"Number of bins[%i]?: ",nbins);
  if((tf1=readl(stdin,&rc))!=0||rc==-1)
    nbins=ti1;

  fprintf(stderr,"Reading '%s'\n--------------------------------\n",file);

  gabby_dbread=2;
  n=dbread_pands(file, &lines, wl1, wl2,zfile,&Z,&T,&isomass,&nT,&nIso,NULL);
  fprintf(stderr,"--------------------------------\n");

  if(n<0)
    transiterror(TERR_SERIOUS,
		 "dbread_pands() return error status %i.\n",n);

  ti1=(int)(log10(n)+1);

  printf("\ndbread_pands() test results:\n"
	  " Chosen wavelength range was from %.2f to %.2f [nm]\n"
	  " %*li lines read\n"
	  " Choosing %i equal-sized bins, the result is\n"
	  ,wl1,wl2,ti1,n,nbins);

  szb=(wl2-wl1)/nbins;
  lp=lines;
  if(!nbins)
    printf("  hmmm, you chose 0 bins!\n");
  for(i=0;i<nbins;i++){
    memset(qb,0,sizeof(*qb)*4);
    endb=wl1+(i+1)*szb;
    //    fprintf(stderr,"KK %g %f\n",lp->wl,endb);
    while((lp->wl)<endb&&lp-lines<n){
      qb[lp->isoid]++;
      lp++;
    }

    printf(" %*i = %i + %i + %i + %i lines shorter than %.3f\n"
	   ,ti1,qb[0]+qb[1]+qb[2]+qb[3],qb[0],qb[1],qb[2],qb[3],endb);
  }

  /*
  printf("\nTemperature points:");
  for(ti1=0;ti1<nT;ti1++)
    printf(" %.1f",T[ti1]);
  printf("\n");

  for(i=0;i<nIso;i++){
    printf("\n%s:",isotope[lines[i].isoid]);
    for(ti1=0;ti1<nT;ti1++)
      printf(" %.3f",Z[i][ti1]);
    printf("\n");
  }
  */

  printf("\nWanna know the value of a single record?\n"
	  "If so, write record number (range 0 - %li), else "
	    "press ^D: "
	  ,n-1);


  while((ans=readl(stdin,&rc))!=0||rc==-1){
    if(ans<n&&ans>=0){
      lp=lines+ans;
      printf("Record Position: %li\nWavelength: %.10g\n"
	      ,lp->recpos,lp->wl);
      printf("Lower Energy Level: %.10g\nLog(gf): %.10g\n"
	      , lp->elow, lp->lgf);
      printf("Isotope Name: %s\n"
	      ,isotope[lp->isoid]);
    }
    else
      printf("\nInvalid record number, so ...");

    printf("\nWanna know the value of another single record?\n"
	    "If so, write the record number (range 0 - %li), else just "
	    "press ^D: "
	    ,n-1);
  }

  printf("\nHave a good day!...\n");

  return EXIT_SUCCESS;

}
