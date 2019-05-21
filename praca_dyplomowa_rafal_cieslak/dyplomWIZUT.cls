\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{dyplomWIZUT}[2017/06/01 Szablon Pracy Dyplomowej na WI ZUT]

%% CHANGELOG
%
% v1.1 [01.06.2017] {M. Tecław}
% * Dokończona konfiguracja layoutu dla kodów źródłowych
% * Implementacja BibLaTeX jako modułu obsługującego budowanie bibliografii
%
% v1.0 [25.04.2017] {M. Tecław}
% * Wersja pierwsza

\def\@skorowidz{off}
\def\@skroty{off}

% Buduj skorowidz
\DeclareOption{skorowidz}{\def\@skorowidz{on}}
\DeclareOption{skroty}{\def\@skroty{on}}

% Fallback
\DeclareOption*{\ClassWarning{dyplomWIZUT}{Nieznana opcja '\CurrentOption'}}
\ProcessOptions\relax

% Load base class (report)
\LoadClass[a4paper,twoside,openany,12pt]{report}

\RequirePackage{xstring}
\RequirePackage{graphicx}
\RequirePackage{fontspec}
%\RequirePackage{amsmath}
\RequirePackage[math-style=ISO]{unicode-math}
\RequirePackage{polyglossia}
\RequirePackage{listings}
\RequirePackage{float}
\RequirePackage{color}
\RequirePackage{csquotes}
\RequirePackage{appendix}
\RequirePackage[
	bibstyle=dyplomWIZUT,
	citestyle=numeric,
	maxnames=6,
	sorting=nyt,
	backend=biber]{biblatex}
\addbibresource{bibliografia.bib}
\RequirePackage[
	unicode,
	xetex,
	bookmarksnumbered
]{hyperref}
\RequirePackage[figure]{hypcap}
\RequirePackage[
	style=super,
	toc,
	nonumberlist,
	acronym,
	nomain,
	nogroupskip,
	nopostdot
]{glossaries}
\loadglsentries{acronyms}
\RequirePackage[
	left 			= 20mm ,
	right 			= 20mm ,
	top 			= 20mm ,
	bottom			= 30mm ,
	bindingoffset	= 15mm
]{geometry}

% Paragraph indent 10mm, skip 0
\setlength\parskip{0mm}%
\setlength\parindent{10mm}%

\setmainlanguage{polish}
\setotherlanguage{english}

% Main document font
\setmainfont{FranklinGothic}[
	Path 			= fonts/ ,
	Extension		= .ttf ,
	UprightFont		= *-Book ,
	ItalicFont 		= *-Book-Italic ,
	BoldFont		= *-Demi ,
	BoldItalicFont	= *-Demi-Italic ]

% Monospaced font used for Listings
\setmonofont{Inconsolata}[
	Path 			= fonts/ ,
	Extension		= .ttf ,
	UprightFont		= *-Regular ,
	BoldFont		= *-Bold ]

\setmathfont{CambriaMath}[
	Path 			= fonts/ ,
	Extension		= .ttf
]

% Redefine font macros according to Design Rules
% Baselineskip 1.366 * size [pt] = 1.2 linespread in Word
\renewcommand*{\LARGE}{\@setfontsize\LARGE{20pt}{27.320pt}}%
\renewcommand*{\Large}{\@setfontsize\Large{17pt}{23.222pt}}%
\renewcommand*{\large}{\@setfontsize\large{14pt}{19.124pt}}%
\renewcommand*{\small}{\@setfontsize\small{11pt}{15.026pt}}%
\renewcommand*{\normalsize}{\@setfontsize\normalsize{12pt}{16.392pt}}%
\renewcommand*{\footnotesize}{\@setfontsize\footnotesize{10pt}{13.660pt}}%
\renewcommand*{\scriptsize}{\@setfontsize\scriptsize{8pt}{10.928pt}}%
\renewcommand*{\tiny}{\@setfontsize\tiny{6pt}{8.196pt}}%
\newcommand*{\on}{on}%

\ifx\@skorowidz\on
	\RequirePackage{makeidx}
	\makeindex
\fi

%
% Configure Listings
%

% Define some gray colors to use in listings
\definecolor{black}{gray}{0}
\definecolor{gray10}{gray}{0.1}
\definecolor{gray20}{gray}{0.2}
\definecolor{gray30}{gray}{0.3}
\definecolor{gray40}{gray}{0.4}
\definecolor{gray50}{gray}{0.5}

% Listings extended charmap - utf8 - pl
\lst@InputCatcodes
\def\lst@DefEC{%
	\lst@CCECUse \lst@ProcessLetter
	^^80^^81^^82^^83^^84^^85^^86^^87^^88^^89^^8a^^8b^^8c^^8d^^8e^^8f%
	^^90^^91^^92^^93^^94^^95^^96^^97^^98^^99^^9a^^9b^^9c^^9d^^9e^^9f%
	^^a0^^a1^^a2^^a3^^a4^^a5^^a6^^a7^^a8^^a9^^aa^^ab^^ac^^ad^^ae^^af%
	^^b0^^b1^^b2^^b3^^b4^^b5^^b6^^b7^^b8^^b9^^ba^^bb^^bc^^bd^^be^^bf%
	^^c0^^c1^^c2^^c3^^c4^^c5^^c6^^c7^^c8^^c9^^ca^^cb^^cc^^cd^^ce^^cf%
	^^d0^^d1^^d2^^d3^^d4^^d5^^d6^^d7^^d8^^d9^^da^^db^^dc^^dd^^de^^df%
	^^e0^^e1^^e2^^e3^^e4^^e5^^e6^^e7^^e8^^e9^^ea^^eb^^ec^^ed^^ee^^ef%
	^^f0^^f1^^f2^^f3^^f4^^f5^^f6^^f7^^f8^^f9^^fa^^fb^^fc^^fd^^fe^^ff%
	^^^^0104^^^^0105^^^^0106^^^^0107^^^^0118^^^^0119^^^^0141^^^^0142%
	^^^^0143^^^^0144^^^^015a^^^^015b^^^^0179^^^^017a^^^^017b^^^^017c%
	^^00}
\lst@RestoreCatcodes

\lstset{
	numberstyle=\color{gray20}\tiny,
	numbers=left,
	tabsize=4,
	numbersep=0pt,
	breakindent=18pt,
	breakatwhitespace,
	breaklines,
	postbreak=\space,
	extendedchars=true,
	columns=[c]flexible,
	keepspaces=true,
	xleftmargin=8pt,
	framexleftmargin=0pt,
	frame=L,
	rulesep=1pt,
	rulesepcolor=\color{black},
	abovecaptionskip=-6pt,
	belowcaptionskip=6pt,
	belowskip=9pt,
	showstringspaces=false,
	basicstyle=\ttfamily\lst@ifdisplaystyle\footnotesize\fi,
	commentstyle=\color{gray40}\ttfamily,
}

\def\lst@PlaceNumber{%
	\makebox[2em][c]{\normalfont\lst@numberstyle{\thelstnumber}}\hspace{.2em}%
}

\renewcommand{\lstlistingname}{Kod źródłowy}
\renewcommand{\lstlistlistingname}{Spis kodów źródłowych}

\newcommand{\code}[3]{
	\lstset{caption={[#1]#1#3\hfill\break\hfill Źródło: #2}}
}

\newcommand{\codeMoje}[2]{
	\lstset{caption={[#1]#1#2\hfill\break\hfill}}
}

\newcommand{\codeinput}[5]{
	\lstset{caption={[#1]#1#3\hfill\break\hfill Źródło: #2}}
	\lstinputlisting[#5]{./code/#4}
}

\setlength{\parskip}{0pt}%
%
% Configure Listings -- END
%

% Acronyms - setup if <skroty> class option active
\ifx\@skroty\on
	\setlength{\glsdescwidth}{0.9\textwidth}
	\renewcommand{\glsnamefont}[1]{\bfseries\boldmath #1}
	\makenoidxglossaries
\fi

%\widowpenalty999999
%\tolerance450
%\pretolerance250
%\hfuzz=1.5pt
%\hbadness1450

% PDF Metadata macro command
\newcommand*{\makemetadata}{
	\StrSubstitute{\@slowakluczowe}{, }{,}[\keywrdspl]
	\StrSubstitute{\@keywords}{, }{,}[\keywrdseng]
	\hypersetup{
		%hidelinks,
		pdfauthor={\@author},
		pdftitle={\@title},
		pdfsubject={Praca dyplomowa \@rodzajpracy},
		pdfkeywords={\keywrdspl,\keywrdseng},
		pdfcreator={XeLaTeX with hyperref},
		pdfdisplaydoctitle={true}
	}
}

%% Command: txtline{fontsize}{text}{verticalSkipAfter}
%% Macro to easily format block of text
\newcommand{\liner}[3][\normalsize]{#1 #2\par\vspace{#3}}
\newcommand{\lineb}[3][\normalsize]{#1{\bfseries #2}\par\vspace{#3}}

%\renewcommand*{\thepage}{\small\csname @arabic\endcsname \c@page}

%% Strona tytułowa
% ---------------- maketitle ----------------
\renewcommand{\maketitle}{
\begin{titlepage}
    \begin{center}

    	% Logo ZUT i WI
		\includegraphics[height=1.8cm]{graphic/logo_ZUT}
		\hfill
		\includegraphics[height=1.8cm,]{graphic/logo_wi_kolor}
		\vskip 55pt

		% Autor
		\lineb[\large]{\@author}{18pt}
		
		% nr albumu, kierunek studiów, specjalność, forma studiów
		\liner{nr albumu: \@nralbumu}{0pt}
		\liner{kierunek studiów: \@kierunek}{0pt}
		%\ifx\@showspec\on
			\liner{specjalność: \@specjalnosc}{0pt}
		%\fi
        \liner{forma studiów: studia \@formastudiow}{0pt}
		\vspace{38pt}
		% Tytuł pracy w języku polskim
		\lineb[\large]{\MakeUppercase{\@title}}{38pt}
		% Tytuł pracy w języku angielskim
		\lineb[\large]{\MakeUppercase{\@tytulang}}{0pt}
		
		% Wypełnienie wolnej przestrzeni
		\vfill 
			
		% Rodzaj pracy
		\liner{Praca dyplomowa \@rodzajpracy}{4pt}
		% Promotor
		\liner{napisana pod kierunkiem:}{4pt}
		\lineb[\large]{\@opiekun}{4pt}
		% Jednostka dyplomująca
		\liner{\@jednostka}{28pt}
	
		\begin{flushleft}
			\liner[\footnotesize]{Data wydania tematu pracy: \@datawydania~r.}{3pt}
			\liner[\footnotesize]{Data złożenia pracy: \@datazlozenia~r.}{18pt}
		\end{flushleft}
		
		\liner{\@miejsce, \@date}{0pt}
		
    \end{center}
\end{titlepage}

\setcounter{footnote}{0}

%% Pusta strona nienumerowana (dla wydruku dwustronnego)
\clearpage\thispagestyle{empty}\null

%% Oświadczenie autora pracy
\clearpage\thispagestyle{empty}

\begin{center}
	\noindent\large\bfseries OŚWIADCZENIE\\ AUTORA PRACY DYPLOMOWEJ\par
\end{center}

\vspace{24pt}

\normalsize
\noindent Oświadczam, że praca {\@rodzajpracy} pn.\par
\noindent ,,\@title''\par
\noindent napisana pod kierunkiem:\par
\noindent \@opiekun\par
\noindent jest w~całości moim samodzielnym autorskim opracowaniem sporządzonym przy wykorzystaniu wykazanej w~pracy literatury przedmiotu i~materiałów źródłowych.\par
\noindent Złożona w dziekanacie Wydziału Informatycznego treść mojej pracy dyplomowej w~formie elektronicznej jest zgodna z~treścią w~formie pisemnej.\par
\vspace{12pt}
\noindent Oświadczam ponadto, że złożona w~dziekanacie praca dyplomowa ani jej fragmenty nie były wcześniej przedmiotem procedur procesu dyplomowania związanych z~uzyskaniem tytułu zawodowego w~uczelniach wyższych.\par
\vspace{72pt}

\footnotesize\hfill................................................\hspace*{36pt}\par
\noindent\hfill podpis dyplomanta\hspace*{55pt}\par
\vspace{12pt}
\normalsize
\noindent Szczecin, dn. \@datazlozenia~r.\par

%% Streszczenie pracy / Abstract
\clearpage\thispagestyle{empty}

\InsertStreszczenieHere%
\subsection*{Słowa kluczowe}%
\normalsize\@slowakluczowe%

\vspace{18pt}

\InsertAbstractHere%
\subsection*{Keywords}%
\normalsize\@keywords%

\cleardoublepage

%% Spis treści
\clearpage\thispagestyle{empty}\small
\tableofcontents

%% Wykaz ważniejszych skrótów i oznaczeń
\ifx\@skroty\on
\glsaddall
\normalsize
\printnoidxglossary[type=acronym,title={Wykaz ważniejszych oznaczeń i~skrótów}]
\fi

%% Garbage collection
\global\let\@title\@empty
\global\let\@author\@empty
\global\let\@date\@empty
\global\let\@datawydania\@empty
\global\let\@datazlozenia\@empty
\global\let\@formastudiow\@empty
\global\let\@rodzajstudiow\@empty
\global\let\@rodzajpracy\@empty
\global\let\@showspec\@empty

\global\let\title\relax
\global\let\author\relax
\global\let\date\relax
\global\let\datawydania\relax
\global\let\datazlozenia\relax
\global\let\formastudiow\relax
\global\let\rodzajpracy\relax
\global\let\rodzajstudiow\relax
\global\let\showspec\relax

\gdef\@showspec{off}
\gdef\@rodzajpracy{inżynierska}
\gdef\@formastudiow{stacjonarne}

\global\let\maketitle\relax

\reset@font\normalsize
}% ---------------- maketitle ----------------

\renewcommand*{\cleardoublepage}{%
	\clearpage%
	\if@twoside\ifodd\c@page\else%
   		\null\thispagestyle{empty}\newpage%
   		\if@twocolumn
   			\hbox{}\newpage
   		\fi
    \fi\fi%
}

%\renewcommand{\tableofcontents}{
%	\if@twocolumn
%		\@restonecoltrue\onecolumn
%	\else
%		\@restonecolfalse
%	\fi
%	\chapter*{
%		\contentsname
%	    \@mkboth{\MakeUppercase\contentsname}{\MakeUppercase\contentsname}
%    }
%	\@starttoc{toc}
%	\if@restonecol
%		\twocolumn
%	\fi
%}

% Redefine numbered chapter to cleardoublepage in openany mode
\def\@chapter[#1]#2{%
\ifnum \c@secnumdepth >\m@ne
\cleardoublepage%
\refstepcounter{chapter}%
\typeout{\@chapapp\space\thechapter.}%
\addcontentsline{toc}{chapter}%
{\protect\numberline{\thechapter}#1}%
\else
\addcontentsline{toc}{chapter}{#1}%
\fi
\chaptermark{#1}%
\addtocontents{lof}{\protect\addvspace{10\p@}}%
\addtocontents{lot}{\protect\addvspace{10\p@}}%
\if@twocolumn
\@topnewpage[\@makechapterhead{#2}]%
\else
\@makechapterhead{#2}%
\@afterheading
\fi}

%
% Definicja zmiennych globalnych
%
\def\author#1{\gdef\@author{#1}}
\def\@author{???\ClassError{dyplomWIZUT}{Brak zdefiniowanego autora pracy}\@ehc}
%
\def\nralbumu#1{\gdef\@nralbumu{#1}}
\def\@nralbumu{???\ClassError{dyplomWIZUT}{Brak numeru albumu}\@ehc}
%
\def\kierunek#1{\gdef\@kierunek{#1}}
\def\@kierunek{???\ClassError{dyplomWIZUT}{Nie podano kierunku studiów}\@ehc}
%
\def\specjalnosc#1{\gdef\@specjalnosc{#1}}
\def\@specjalnosc{???\ClassError{dyplomWIZUT}{Nie podano specjalności}\@ehc}
%
\def\jednostka#1{\gdef\@jednostka{#1}}
\def\@jednostka{???\ClassError{dyplomWIZUT}{Nie podano jednostki dyplomującej}\@ehc}
%
\def\opiekun#1{\gdef\@opiekun{#1}}
\def\@opiekun{???\ClassError{dyplomWIZUT}{Brak danych opiekuna pracy}\@ehc}
%
\def\miejsce#1{\gdef\@miejsce{#1}}
\def\@miejsce{Szczecin}
%
\def\keywords#1{\gdef\@keywords{#1}}
\def\@keywords{???\ClassError{dyplomWIZUT}{Brak słów kluczowych w języku angielskim}\@ehc}
%
\def\slowakluczowe#1{\gdef\@slowakluczowe{#1}}
\def\@slowakluczowe{???\ClassError{dyplomWIZUT}{Brak słów kluczowych}\@ehc}
%
\def\tytulang#1{\gdef\@tytulang{#1}}
\def\@tytulang{???\ClassError{dyplomWIZUT}{Brak tytułu pracy w języku angielskim}\@ehc}
%
\def\datazlozenia#1{
	\gdef\@datazlozenia{#1}
	\StrRight{\@datazlozenia}{4}[\@date]
}
\def\@datazlozenia{???\ClassError{dyplomWIZUT}{Nie podano daty złożenia pracy w dziekanacie}\@ehc}
%
\def\datawydania#1{\gdef\@datawydania{#1}}
\def\@datawydania{???\ClassError{dyplomWIZUT}{Nie podano daty wydania tematu pracy}\@ehc}
%
\def\rodzajstudiow#1{
	\gdef\@rodzajstudiow{#1}
	\def\@forma{N}
	\def\@stopien{2}
	\StrLeft{\@rodzajstudiow}{1}[\@tfrm]
	\StrRight{\@rodzajstudiow}{1}[\@tstp]
	
	\ifx\@forma\@tfrm
		\gdef\@formastudiow{niestacjonarne}
	\fi
	
	\ifx\@stopien\@tstp
		\gdef\@rodzajpracy{magisterska}
		\gdef\@showspec{on}
	\fi
	
	\let\@forma\relax
	\let\@tfrm\relax
	\let\@stopien\relax
	\let\@tstp\relax
}
\def\@rodzajstudiow{???\ClassError{dyplomWIZUT}{Nie podano rodzaju studiów [S1/S2/N1/N2]}\@ehc}
%
\gdef\@showspec{off}
\gdef\@rodzajpracy{inżynierska}
\gdef\@formastudiow{stacjonarne}

% Definicja środowiska Abstract oraz Streszczenie
\newbox{\abspagebox}
\newbox{\strpagebox}
\def\InsertAbstractHere{\unvbox\abspagebox}
\def\InsertStreszczenieHere{\unvbox\strpagebox}

\renewenvironment{abstract}{
	\global\setbox\abspagebox\vbox\bgroup\reset@font
	\subsection*{Abstract}}{\egroup}

\newenvironment{streszczenie}{
	\global\setbox\strpagebox\vbox\bgroup\reset@font
	\subsection*{Streszczenie pracy}}{\egroup}

% Redefinicja elementów spisu treści
\renewcommand*{\@seccntformat}[1]{\csname the#1\endcsname.\enspace}
\renewcommand*{\numberline}[1]{\hb@xt@\@tempdima{#1.\hfil}}
\renewcommand*\l@chapter[2]{%
\ifnum \c@tocdepth >\m@ne
	\vskip 6pt \addpenalty{-\@highpenalty} \setlength\@tempdima{15pt}%
	\begingroup
		\parindent \z@ \rightskip \@pnumwidth
		\parfillskip -\@pnumwidth
		\leavevmode \bfseries
		\advance\leftskip\@tempdima
		\hskip -\leftskip
		#1\nobreak\mdseries
		\leaders\hbox{$\m@th \mkern \@dotsep mu\hbox{.}\mkern \@dotsep mu$}\hfill
		\nobreak\hb@xt@\@pnumwidth{\hss #2}\par
		\penalty\@highpenalty
	\endgroup
\fi
}

%
\renewcommand*{\@makechapterhead}[1]{%
	\vspace*{25mm}%
	{%
		\parindent \z@ \raggedright \reset@font%
		\ifnum \c@secnumdepth >\m@ne%
			\large\bfseries\MakeUppercase{\@chapapp}\space\thechapter\par\nobreak%
			\vskip 18mm%
		\fi%
		\interlinepenalty\@M%
		\LARGE\bfseries #1\par\nobreak%
		\vskip 20mm%
	}%
}%

\renewcommand{\@makeschapterhead}[1]{%
	{\parindent \z@ \raggedright \reset@font \interlinepenalty \@M%
	\LARGE\bfseries #1\par\nobreak\vskip 12mm}%
}

\renewcommand{\section}{
	\@startsection{section}{1}{\z@}
	{-6mm \@plus -.75mm \@minus -.25mm}
	{4mm}%{4mm \@plus .5mm}
	{\Large\bfseries\raggedright}}

\renewcommand{\subsection}{
	\@startsection{subsection}{2}{\z@}
	{-5mm \@plus -.5mm \@minus -.25mm}
	{4mm}%{4mm \@plus .5mm}
	{\large\bfseries\raggedright}}

\renewcommand{\subsubsection}{
	\@startsection{subsubsection}{3}{\z@}
    {-4mm \@plus -.25mm \@minus -.25mm}
    {3mm}%{3mm \@plus .3mm}
    {\normalsize\bfseries\raggedright}}

\renewcommand{\paragraph}{???\ClassError{dyplomWIZUT}{Zbyt dużo śródtytułów}\@ehc}

\newcommand{\rysunekb}[4]{
\begin{figure}[htb]
	\centering\fbox{\includegraphics[height=6cm,width=0.8\textwidth,keepaspectratio]{graphic/#1}}\\
	\caption{#2}\source{#3}
\end{figure}
}
%
\newcommand{\rysunek}[4]{
\begin{figure}[htb]
	\centering\includegraphics[height=6cm,width=0.8\textwidth,keepaspectratio]{graphic/#1}\\
	\caption{#2}\source{#3}
\end{figure}
}
%
\newcommand{\rysunekduzy}[4]{
\begin{figure}[htb]
	\centering\includegraphics[height=10cm,width=1.0\textwidth,keepaspectratio]{graphic/#1}\\
	\caption{#2}\source{#3}
\end{figure}
}

%
\newcommand{\tabela}[3]{
\begin{table}[ht]
	\caption{#1}\source{#2}
	\centering\small\fbox{#3}
\end{table}
}

\newcommand{\tabelabezzrodla}[2]{
\begin{table}[ht]
	\caption{#1}
	\centering\small\fbox{#2}
\end{table}
}

% Skorowidz: dodanie kresek w pozycjach skorowidza stopnia drugiego
\renewenvironment{theindex}{%
	% potrzebne do prawidłowego działania zakładek w dokumencie PDF
   \clearpage\phantomsection\refstepcounter{chapter}%
   \columnseprule \z@ \columnsep 35\p@%
   \twocolumn[\@makeschapterhead{Skorowidz}]%
   \@mkboth{\indexname}{\indexname}%
   \addcontentsline{toc}{chapter}{Skorowidz}%
   \thispagestyle{plain}\parindent\z@%
   \parskip\z@ \@plus .3\p@\relax%
   \let\item\@idxitem%
}
{\clearpage}
   
%\def\indexemdashes{%
\def\@idxitem{\par\hangindent 0pt}
\def\subitem{\par\hangindent 0pt --- }
\def\subsubitem{\par\hangindent 0pt --- --- }
\def\indexspace{\par\vskip 10pt plus 5pt minus 3pt\relax}%}
%\indexemdashes

% Wprowadzenie
\newenvironment{wprowadzenie}{\chapter*{Wprowadzenie}\addcontentsline{toc}{chapter}{Wprowadzenie}}{}

% Cel
\newcommand{\cel}[1]{
    \bigskip\bigskip
    \noindent\textbf{\large Cel pracy\label{cel}}
    \bigskip\newline\noindent\normalsize{#1}
}

% Zakres
\newcommand{\zakres}[1]{
    \bigskip\bigskip
    \noindent\textbf{\large Zakres pracy\label{zakres}}
    \bigskip\newline\noindent\normalsize{#1}\clearpage
}
%
\newenvironment{zakonczenie}
{\chapter*{Zakończenie}\addcontentsline{toc}{chapter}{Zakończenie}}
{\clearpage}
%
\renewcommand*{\thefigure}{\ifnum\c@chapter>\z@\thechapter.\fi\@arabic\c@figure}
%
\renewcommand*{\fnum@figure}{\figurename\nobreakspace\thefigure}
%
\renewcommand\listoffigures{
	\reset@font
    \chapter*{
    	\listfigurename
      	\@mkboth{\MakeUppercase\listfigurename}{\MakeUppercase\listfigurename}
	}
    \addcontentsline{toc}{chapter}{\listfigurename}
    \@starttoc{lof}
}
%
\renewcommand*{\fnum@table}{\tablename\nobreakspace\thetable}
%
\renewcommand\listoftables{%
	\normalfont\selectfont
    \chapter*{
    	\listtablename
    	\@mkboth{\MakeUppercase\listtablename}{\MakeUppercase\listtablename}
  	}
    \@starttoc{lot}
    \addcontentsline{toc}{chapter}{\listtablename}
}

\setlength\abovecaptionskip{-5\p@}
\setlength\belowcaptionskip{0\p@}

\renewcommand{\@makecaption}[2]{
	\vskip-\abovecaptionskip\reset@font
	\small\textbf{#1.} #2\hfill\normalsize\par
	\vskip\belowcaptionskip
}

\newcommand*{\source}[1]{
	\par\vspace{-.7\baselineskip}
	\begin{flushleft}
		\reset@font\small Źródło:~#1%
	\end{flushleft}
}

\newcommand*{\listoflistings}{
	\lstlistoflistings
	\addcontentsline{toc}{chapter}{Spis kodów źródłowych}
}

\endinput
