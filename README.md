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

After changing a lesson's assembly source, rebuild its floppy image and copy it
into the documentation site:

```sh
pnpm run playground:lesson-00
pnpm run playground:lesson-01
```

Refresh every browser playground:

```sh
pnpm run playground:build
```

Run the complete local check:

```sh
pnpm test
```

This refreshes every playground image and performs the same Starlight build
used by GitHub Pages.

## Add a browser playground

Place a bootable image in `public/assets/playground/`, then add the reusable
playground host to any tutorial Markdown file:

```html
<div
  data-aos-v86-playground
  lesson="lesson-00"
  title="Lesson 00 · Boot sector"
  expected-output="Peace be upon you!"
></div>
```

The shared script upgrades the Markdown-safe `<div>` to an
`<aos-v86-playground>` custom element. By default, `lesson="lesson-00"` loads
`lesson-00.img`. Use the optional `image`, `description`, or `memory-mib`
attributes when a lesson needs different settings.
