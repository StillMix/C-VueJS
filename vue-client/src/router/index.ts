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
      import(/* webpackChunkName: "draw" */ "../views/DrawView.vue"),
  },
  {
    // Добавляем новый маршрут для Рисовать изображения
    path: "/edit/:imageName",
    name: "edit-image",
    component: () =>
      import(/* webpackChunkName: "edit" */ "../views/EditImageView.vue"),
    props: true, // Передаем параметр как props
  },
  {
    // Перехватываем некорректные URL с /draw/... и перенаправляем на /draw
    path: "/draw/:pathMatch(.*)*",
    redirect: { name: "draw" },
  },
];

const router = createRouter({
  history: createWebHashHistory(),
  routes,
});

export default router;
