<template>
    <div id="app">
        <Header :user="users[userId]"/>
        <Middle :posts="posts" :users="users"/>
        <Footer :numPosts="Object.keys(posts).length" :numUsers="Object.keys(users).length"/>
    </div>
</template>

<script>
import Header from "@/components/Header.vue";
import Middle from "@/components/Middle.vue";
import Footer from "@/components/Footer.vue";

const strIsBlank = (s) => { return s === null || s === undefined || /^\s*$/.test(s) };

const nextId = (obj) => {
    const existingIds = Object.keys(obj);
    return existingIds.length > 0 ? Math.max(...existingIds) + 1 : 0;
};

export default {
    name: 'App',
    components: {
        Footer,
        Middle,
        Header
    },
    data: function () {
        return this.$root.$data;
    },
    beforeCreate() {
        this.$root.$on("onEnter", (login, password) => {
            if (password === "") {
                this.$root.$emit("onEnterValidationError", "Password is required");
                return;
            }

            const users = Object.values(this.users).filter(u => u.login === login);
            if (users.length === 0) {
                this.$root.$emit("onEnterValidationError", "No such user");
            } else {
                this.userId = users[0].id;
                this.$root.$emit("onChangePage", "Index");
            }
        });

         this.$root.$on("onRegister", (login, name, password) => {
             if (this.userId) {
                this.$root.$emit("onRegisterValidationError", "Already logged in");
             } else {
                if (!login || !/^[a-z]{3,16}$/.test(login)) {
                    this.$root.$emit("onRegisterValidationError", "Invalid login");
                } else if (Object.values(this.users).filter(u => u.login === login).length !== 0) {
                    this.$root.$emit("onRegisterValidationError", "Login already exists");
                } else if (strIsBlank(name) || name.length > 32) {
                    this.$root.$emit("onRegisterValidationError", "Invalid name");
                } else if (strIsBlank(password) || password.length < 3) {
                    this.$root.$emit("onRegisterValidationError", "Invalid password");
                } else {
                    const id = nextId(this.users);
                    this.$root.$set(this.users, id, {
                        id, login, name: name.trim(), admin: false,
                    });
                    this.$root.$emit("onChangePage", "Enter");
                }
            }
        });

        this.$root.$on("onLogout", () => this.userId = null);

        this.$root.$on("onWritePost", (title, text) => {
            if (this.userId) {
                if (strIsBlank(title) || title.length < 5) {
                    this.$root.$emit("onWritePostValidationError", "Title is too short");
                } else if (strIsBlank(text) || text.length < 10) {
                    this.$root.$emit("onWritePostValidationError", "Text is too short");
                } else {
                    const id = nextId(this.posts);
                    this.$root.$set(this.posts, id, {
                        id, title, text, userId: this.userId
                    });
                }
            } else {
                this.$root.$emit("onWritePostValidationError", "No access");
            }
        });

        this.$root.$on("onEditPost", (id, text) => {
            if (this.userId) {
                if (!id) {
                    this.$root.$emit("onEditPostValidationError", "ID is invalid");
                } else if (strIsBlank(text) || text.length < 10) {
                    this.$root.$emit("onEditPostValidationError", "Text is too short");
                } else {
                    let posts = Object.values(this.posts).filter(p => p.id === parseInt(id));
                    if (posts.length) {
                        posts.forEach((item) => {
                            item.text = text;
                        });
                    } else {
                        this.$root.$emit("onEditPostValidationError", "No such post");
                    }
                }
            } else {
                this.$root.$emit("onEditPostValidationError", "No access");
            }
        });
    }
}
</script>

<style>
#app {

}
</style>
