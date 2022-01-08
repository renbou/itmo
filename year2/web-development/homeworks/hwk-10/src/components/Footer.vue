<template>
    <footer>
        <div>
            <a href="/">Codeforces</a>
            <span> &copy; 2021 by Mike Mirzayanov</span>
        </div>
        <div class="stats">
            <span>Registered users: {{ numUsers }}</span>
            <span>Number of posts: {{ numPosts }}</span>
        </div>
    </footer>
</template>

<script>
export default {
    name: "Footer",
    props: ["numUsers", "numPosts"]
}
</script>

<style scoped>
    footer > div {
        margin-bottom: 0.5rem;
    }
    footer > div:last-child {
        margin-bottom: 0;
    }
    .stats > span {
        margin-right: 1rem;
    }
    .stats > span:last-child {
        margin-right: 0;
    }
</style>