# AOS Kernel Tutorials

An archive of operating-system development tutorials originally written in Arabic,
with progressively published English editions.

- [Read the English tutorials](https://yelouafi.github.io/aos/)
- [View translation progress](https://yelouafi.github.io/aos/translation-status/)

The historical PDFs, HTML exports, assembly programs, and kernel source remain in
their original lesson directories.

## Preview the documentation locally

Install the Node.js dependencies once:

```sh
npm install
```

Then start the local site:

```sh
npm run dev
```

Open <http://127.0.0.1:4321/aos/>. Astro watches the documentation files and
reloads the browser as you edit them.

To preview the exact production build:

```sh
npm run build
npm run preview
```

## Playground commands

After changing the Lesson 00 boot-sector source, rebuild its floppy image and
copy it into the documentation site:

```sh
npm run playground:lesson-00
```

Run the complete local check:

```sh
npm test
```

This refreshes the Lesson 00 playground image and performs the same Starlight
build used by GitHub Pages.
