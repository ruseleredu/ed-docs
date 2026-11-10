import { themes as prismThemes } from "prism-react-renderer";
import type { Config } from "@docusaurus/types";
import type * as Preset from "@docusaurus/preset-classic";

import remarkMath from "remark-math";
import rehypeKatex from "rehype-katex";
// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

import footer from "./footer";
import navbarItems from "./navbar";

const config: Config = {
  title: "ELT82F - Eletrônica Digital",
  tagline:
    "Aplicar princípios da álgebra de Boole. Implementar circuitos lógicos combinacionais. Aplicar circuitos integrados. Implementar circuitos sequenciais.",
  favicon: "img/favicon.ico",

  // Future flags, see https://docusaurus.io/docs/api/docusaurus-config#future
  future: {
    v4: true, // Improve compatibility with the upcoming Docusaurus v4
    faster: true, // Enable all the optimizations that will be part of Docusaurus v4
  },

  // Set the production url of your site here
  url: "https://ruseleredu.github.io",
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: "/ed-docs/",
  trailingSlash: false,

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: "ruseleredu", // Usually your GitHub org/user name.
  projectName: "ed-docs", // Usually your repo name.
  deploymentBranch: "gh-pages",

  onBrokenLinks: "throw",

  markdown: {
    mermaid: true, // Diagrams can be rendered using Mermaid in a code block.
    hooks: {
      onBrokenMarkdownLinks: "warn", // or 'throw'
    },
  },
  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: "pt-BR",
    locales: ["pt-BR"],
  },

  themes: ["@docusaurus/theme-mermaid"],

  // Include the KaTeX CSS in your config under stylesheets:
  /*
  stylesheets: [
    {
      href: "https://cdn.jsdelivr.net/npm/katex@0.16.45/dist/katex.min.css",
      type: "text/css",
      integrity:
        "sha384-4oq/auzzmx0i9ZQW815S9u/s+w/VYRSz/0auctaq99qhFt2YKlaMiLdXBaODJHxZ",
      crossorigin: "anonymous",
    },
  ],
*/
  stylesheets: [
    {
      href: '/katex/katex.min.css', // No need for integrity/crossorigin when local
      type: "text/css",
    },
  ],

  presets: [
    [
      "classic",
      {
        docs: {
          sidebarPath: "./sidebars.ts",
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl: "https://github.com/ruseleredu/ed-docs/edit/main/",
          remarkPlugins: [remarkMath],
          rehypePlugins: [rehypeKatex],

          showLastUpdateAuthor: true,
          showLastUpdateTime: true,
        },
        blog: {
          showReadingTime: true,
          feedOptions: {
            type: ["rss", "atom"],
            xslt: true,
          },
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl: "https://github.com/ruseleredu/ed-docs/edit/main/",
          // Useful options to enforce blogging best practices
          onInlineTags: "warn",
          onInlineAuthors: "warn",
          onUntruncatedBlogPosts: "warn",
        },
        theme: {
          customCss: "./src/css/custom.css",
        },
      } satisfies Preset.Options,
    ],
  ],

  plugins: [
    [
      "@docusaurus/plugin-content-docs",
      /** @type {import('@docusaurus/plugin-content-docs').Options} */
      {
        id: "lab", // Unique ID for this docs instance
        path: "lab-docs", // Path to your LAB docs folder
        routeBasePath: "lab", // Base URL for these docs (e.g., yoursite.com/lab/...)
        sidebarPath: require.resolve("./labsidebars.ts"), // Separate sidebar for LAB docs
        // ... other options specific to your LAB docs
        showLastUpdateAuthor: true,
        showLastUpdateTime: true,
      },
    ],
    [
      "@docusaurus/plugin-content-docs",
      /** @type {import('@docusaurus/plugin-content-docs').Options} */
      {
        id: "ead", // Unique ID for this docs instance
        path: "ead-docs", // Path to your EaD docs folder
        routeBasePath: "ead", // Base URL for these docs (e.g., yoursite.com/ead/...)
        sidebarPath: require.resolve("./eadsidebars.ts"), // Separate sidebar for EaD docs
        // ... other options specific to your EaD docs
        showLastUpdateAuthor: true,
        showLastUpdateTime: true,
      },
    ],
    [
      "@docusaurus/plugin-content-docs",
      /** @type {import('@docusaurus/plugin-content-docs').Options} */
      {
        id: "utfpr", // Unique ID for this docs instance
        path: "utfpr", // Path to your API docs folder
        routeBasePath: "utfpr", // Base URL for these docs (e.g., yoursite.com/api/...)
        sidebarPath: require.resolve("./sidebarsutfpr.ts"), // Separate sidebar for API docs
        // 👇 Add this line for the last update time
        showLastUpdateAuthor: true,
        showLastUpdateTime: true,
        // ... other options specific to your API docs
      },
    ],
    [
      "@docusaurus/plugin-content-docs",
      /** @type {import('@docusaurus/plugin-content-docs').Options} */
      {
        id: "pjts", // Unique ID for this docs instance
        path: "pjts-docs", // Path to your API docs folder
        routeBasePath: "pjts", // Base URL for these docs (e.g., yoursite.com/api/...)
        sidebarPath: require.resolve("./sidebarspjts.ts"), // Separate sidebar for API docs
        // 👇 Add this line for the last update time
        showLastUpdateAuthor: true,
        showLastUpdateTime: true,
        // ... other options specific to your API docs
      },
    ],
    [
      "@cmfcmf/docusaurus-search-local",
      {
        // Options here
      },
    ],
  ],

  themeConfig: {
    // Replace with your project's social card
    image: "img/Banner-ELT72B.png",
    colorMode: {
      respectPrefersColorScheme: true,
    },
    navbar: {
      title: "ELT82F",
      logo: {
        alt: "ELT82F Logo",
        src: "img/logo.svg",
      },
      items: navbarItems,
    },
    footer: footer,
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
      additionalLanguages: [
        "autohotkey",
        "arduino",
        "apacheconf",
        "nginx",
        "bash",
        "matlab",
        "powershell",
        "ini",
        "latex",
      ],
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
