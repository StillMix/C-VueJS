// vue-client/src/router/index.ts
import { createRouter, createWebHashHistory, RouteRecordRaw } from "vue-router";
import HomeView from "../views/HomeView.vue";

const routes: Array<RouteRecordRaw> = [
  {
    path: "/",
    name: "home",
    component: HomeView,
  },
  {
    path: "/draw",
    name: "draw",
    component: () =>
      import(/* webpackChunkName: "about" */ "../views/DrawView.vue"),
  },
  {
    path: "/edit/:image",
    name: "edit",
    component: () =>
      import(/* webpackChunkName: "editor" */ "../views/ImageEditorView.vue"),
    props: true,
  },
];

const router = createRouter({
  history: createWebHashHistory(),
  routes,
});

export default router;
