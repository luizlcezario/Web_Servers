

<div>
<p align="center">
	<a href="https://www.42sp.org.br/">
		<img src="./.github/42.png" alt="42" width="500"/> 
	</a>
</p>
</div>
<p align="center">	
   <a href="https://www.linkedin.com/in/luiz-lima-cezario/">
      <img alt="Nokstella" src="https://img.shields.io/badge/-luizlcezario-682998?style=flat&logo=Linkedin&logoColor=white" />
   </a>

  <a aria-label="Completed" href="https://www.42sp.org.br/">
    <img src="https://img.shields.io/badge/42.WebServer-682998?logo="></img>
  </a>
  <a href="https://github.com/luizlcezario/Web_Servers/commits/master">
    <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/luizlcezario/Web_Servers?color=682998">
  </a> 

  <a href="https://github.com/luizlcezario/Web_Servers/stargazers">
    <img alt="Stargazers" src="https://img.shields.io/github/stars/luizlcezario/Web_Servers?color=682998&logo=github">
  </a>
</p>

<div align="center">
  <sub>WebServer of 42. Make with ❤︎ for
        <a href="https://github.com/luizlcezario">Luiz Cezario</a>  
  </sub>
</div>

# Transcendece

This project is a simple implementation of Nginx with non Block I/O system, the original was in C++, but for study I made in Go to.

## Idea

This project We need to read a file in any format but with especfication much similar to an nginx config where you can set Port, Doamin, Path with diferente execution for many webSites in just one file, the program must read validate and execute what this config needs, and the principal mechanical that was implementad is:

* serving static files
* redirect for other paths our webPages
* automatic error pages
* indexing static files page for folder
* proxy pass for other services
* upload and dowloand of files without limit or with limit.
* support the UPDATE,GET,POST,DELETE method
* support https

## Test

	```bash
	$> make

	```
    After you can open you browser open your browser and acess the https://localhost:443

> This project involves undertaking tasks you have never done before. Remember the beginning of your journey in computer science. Look at you now; it’s time to shine!"

Give ⭐️ if you like this project, this will help me!
