import { defineConfig } from "astro/config";
import starlight from "@astrojs/starlight";

const base = "/aos";

export default defineConfig({
  site: "https://yelouafi.github.io",
  base,
  integrations: [
    starlight({
      title: "AOS Kernel Tutorials",
      description:
        "English translations of the AOS operating-system development tutorials.",
      social: [
        {
          icon: "github",
          label: "GitHub",
          href: "https://github.com/yelouafi/aos",
        },
      ],
      editLink: {
        baseUrl: "https://github.com/yelouafi/aos/edit/main/",
      },
      favicon: "/favicon.png",
      customCss: ["./src/styles/playground.css"],
      head: [
        {
          tag: "script",
          attrs: {
            src: `${base}/javascripts/playground.js`,
            defer: true,
          },
        },
      ],
      sidebar: [
        { slug: "index", label: "Home" },
        {
          label: "Tutorials",
          items: [
            { slug: "tutorials", label: "Overview" },
            {
              slug: "tutorials/00-writing-a-boot-sector",
              label: "00 - Writing Your First Boot Sector",
            },
            {
              slug: "tutorials/01-loading-the-kernel",
              label: "01 - Loading the Kernel",
            },
            {
              slug: "tutorials/02-entering-protected-mode",
              label: "02 - Entering Protected Mode",
            },
            {
              slug: "tutorials/03-starting-a-cpp-kernel",
              label: "03 - Starting a C++ Kernel",
            },
          ],
        },
        { slug: "translation-status", label: "Translation status" },
      ],
      lastUpdated: true,
    }),
  ],
});
