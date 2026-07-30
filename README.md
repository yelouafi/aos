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
pnpm install
```

Then start the local site:

```sh
pnpm run dev
```

Open <http://127.0.0.1:4321/aos/>. Astro watches the documentation files and
reloads the browser as you edit them.

To preview the exact production build:

```sh
pnpm run build
pnpm run preview
```

## Playground commands

After changing lesson source, rebuild every registered playground artifact:

```sh
pnpm run playground:build
```

To rebuild only one lesson, pass its playground ID:

```sh
pnpm run playground:build -- lesson-01
```

Run the complete local check:

```sh
pnpm test
```

This refreshes every playground artifact and performs the same Starlight build
used by GitHub Pages.

Install Chromium once, then run the browser smoke tests:

```sh
pnpm exec playwright install chromium
pnpm run test:e2e
```

## Add a browser playground

Place a bootable floppy image or Multiboot ELF in
`public/assets/playground/`, then add the reusable playground host to any
tutorial Markdown file:

```html
<div
  data-aos-v86-playground
  lesson="lesson-00"
  title="Lesson 00 · Boot sector"
  expected-output="Peace be upon you!"
></div>
```

The shared script upgrades the Markdown-safe `<div>` to an
`<aos-v86-playground>` custom element. Floppy images are the default. Set
`media="multiboot"` and `artifact="lesson-03.elf"` for a directly loaded
Multiboot kernel. The optional `description` and `memory-mib` attributes adjust
the presentation and virtual machine.

Register each playground in `playgrounds.json`. The same manifest drives
artifact building and the browser smoke tests.
