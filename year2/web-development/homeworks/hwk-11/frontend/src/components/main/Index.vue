<template>
    <div>
        <article v-for="post in posts" :key="post.id">
            <div class="title"> {{ post.title }}</div>
            <div class="information" style="display: flex; justify-content: space-between">
                <span>By {{ post.user.login }} </span>
            </div>
            <div class="body">{{ post.text }}</div>
            <ul class="attachment"></ul>
            <div class="footer">
                <div class="left">
                    <img src="../../assets/img/voteup.png" title="Vote Up" alt="Vote Up"/>
                    <span class="positive-score">+173</span>
                    <img src="../../assets/img/votedown.png" title="Vote Down" alt="Vote Down"/>
                </div>
                <div class="right">
                    <img src="../../assets/img/date_16x16.png" title="Publish Time" alt="Publish Time"/>
                    {{ post.creationTime }}
                    <img src="../../assets/img/comments_16x16.png" title="Comments" alt="Comments"/>
                    <a href="#">0</a>
                </div>
            </div>
        </article>
    </div>
</template>

<script>
export default {
    name: "Index",
    props: ["posts"],
    beforeCreate() {
        this.$root.$emit("onUpdatePosts");
    },
}
</script>

<style scoped>

</style>