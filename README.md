# Website

This website is built using [Docusaurus](https://docusaurus.io/), a modern static website generator.

## Installation
- https://www.youtube.com/watch?v=ALDuHAi4F4A
  
```bash
npx create-docusaurus@latest ed-docs classic --typescript
```

```bash
git init
git add .
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/ruseleredu/ed-docs.git
git push -u origin main
```

[Diagrams](https://docusaurus.io/docs/markdown-features/diagrams)
- https://www.youtube.com/watch?v=EACrMUdoYto
  
```bash
npm install --save @docusaurus/theme-mermaid
```

[Math Equations](https://docusaurus.io/docs/markdown-features/math-equations)
- https://www.youtube.com/watch?v=oztjeloRbvU
  
```bash
npm install --save remark-math@6 rehype-katex@7
```

## Clone repo

```bash
git clone https://github.com/ruseleredu/ed-docs.git
cd ed-docs
npm i
```

## Local Development

```bash
npm run start
```

This command starts a local development server and opens up a browser window. Most changes are reflected live without having to restart the server.

## Build

```bash
npm run build
```

This command generates static content into the `build` directory and can be served using any static contents hosting service.

```bash
npm run serve
```

## Deployment

```bash
set GIT_USER=ruseleredu
npm run deploy
```

If you are using GitHub pages for hosting, this command is a convenient way to build the website and push to the `gh-pages` branch.


## Upgrade Docusaurus

To upgrade Docusaurus packages with the latest version, run the following command:

```bash
npm i @docusaurus/core@latest @docusaurus/preset-classic@latest @docusaurus/theme-mermaid@latest @docusaurus/module-type-aliases@latest @docusaurus/tsconfig@latest @docusaurus/types@latest
```

To ensure accurate Baseline data, please update:
```bash
npm i baseline-browser-mapping@latest -D
```

```bash
npm update
```

```bash
npm audit fix
```

Run Automation Script for katex

```bash
node scripts/download-katex.js
```


Run Automation Script for Tinkercad

```bash
node scripts/download-circuits.js
```

```bash
node scripts/pipeline.js
```

Scan a single folder:

```bash
node scripts/download-images.js docs/tutorials
```


To clear the Rspack cache via the CLI:

```bash
rmdir /s /q node_modules\.cache\rspack
```
