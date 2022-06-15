const { description, descriptionES, descriptionFR } = require("../../package");

module.exports = {
  base: "/liquid-simulator-godot/",
  title: "LiquidSim",
  description: description,
  /**
   * Extra tags to be injected to the page HTML `<head>`
   *
   * ref：https://v1.vuepress.vuejs.org/config/#head
   */
  head: [
    ["meta", { name: "theme-color", content: "#3eaf7c" }],
    ["meta", { name: "apple-mobile-web-app-capable", content: "yes" }],
    [
      "meta",
      { name: "apple-mobile-web-app-status-bar-style", content: "black" },
    ],
    ["link", { rel: "icon", href: "/img/icon.png" }],
  ],
  alias: {
    "@assets": "../assets/",
    "@assets2": "../../assets",
  },
  themeConfig: {
    logo: "/img/icon.png",
    repo: "aj-wi/liquid-simulator-godot",
    docsDir: "docs",
    editLinks: true,
    editLinkText: "Edit this page on GitHub",
    nav: [
      { text: "Guide", link: "/guide/" },
      { text: "Demo", link: "/game/" },
    ],
    sidebarDepth: 3,
    sidebar: {
      "/guide/": [""],
    },
    smoothScroll: true,
    locales: {
      "/es/": {
        selectText: "Idiomas",
        editLinkText: "Editar esta página en GitHub",
        nav: [
          { text: "Guia", link: "/es/guide/" },
          { text: "Demo", link: "/es/game/" },
        ],
        algolia: {},
      },
      "/fr/": {
        selectText: "langues",
        editLinkText: "Modifier cette page sur GitHub",
        nav: [
          { text: "Guide", link: "/fr/guide/" },
          { text: "Démo", link: "/fr/game/" },
        ],
        algolia: {},
      },
    },
  },
  /**
   * Apply plugins，ref：https://v1.vuepress.vuejs.org/zh/plugin/
   */
  plugins: ["@vuepress/plugin-back-to-top", "@vuepress/plugin-medium-zoom"],
  locales: {
    // The key is the path for the locale to be nested under.
    // As a special case, the default locale can use '/' as its path.
    "/": {
      lang: "English",
    },
    "/es/": {
      lang: "Spanish",
      description: descriptionES,
    },
    "/fr/": {
      lang: "French",
      description: descriptionFR,
    },
  },
};
